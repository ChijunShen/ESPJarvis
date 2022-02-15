#include "ESPJarvis.h"

/**
 * Constructor
 * @param sServer IP Address of the MQTT Broker
 * @param Port Port number of the MQTT Broker
 * @brief initialize the class with information of the MQTT Broker
 */
ESPJarvis::ESPJarvis(String sServer, int iPort) {
    sBrokerServer = sServer;
    iBrokerPort = iPort;
    initScreenBuffer();    
}

ESPJarvis::ESPJarvis(Adafruit_SSD1306 &screen, String sServer, int iPort){
    _screen_ssd1306 = &screen;
    sBrokerServer = sServer;
    iBrokerPort = iPort;
    _bUseSSD1306 = true;
    initScreenBuffer();    
}

ESPJarvis::ESPJarvis(Adafruit_ST7735 &screen, String sServer, int iPort){
    _screen_st7735 = &screen;
    sBrokerServer = sServer;
    iBrokerPort = iPort;
    _bUseST7735 = true;
    initScreenBuffer();    
}

void ESPJarvis::attachScreen(Adafruit_SSD1306 &screen){
    _screen_ssd1306 = &screen;
    _bUseSSD1306 = true;
}

void ESPJarvis::attachScreen(Adafruit_ST7735 &screen){
    _screen_st7735 = &screen;
    _bUseST7735 = true;
    _screen_st7735->fillScreen(ST77XX_MAGENTA);
}

void ESPJarvis::setClientData(String sClientID, String sClientName, String sClientPassword){
    sBrokerClientID = sClientID;
    sBrokerClientName = sClientName;
    sBrokerClientPassword = sClientPassword;
}

void ESPJarvis::MQTTCallbackFunction(char *topic, byte *payload, unsigned int length) {
    String sTopic = String(topic);
    for (int i = 0; i < length; i++) {
        cMQTTPayload[i]= (char) payload[i];
    }
    cMQTTPayload[length] = 0;
    if(sTopic.indexOf("cpuload")>0){
        sCpuLoad = String(cMQTTPayload);        
    }else if(sTopic.indexOf("temp")>0){
        sCpuTemp = String(cMQTTPayload);     
    }else if(sTopic.indexOf("disk")>0){
        sDiskUsage = String(cMQTTPayload);     
    }else if(sTopic.indexOf("gpu")>0){
        sGpuFreq = String(cMQTTPayload);     
    }else if(sTopic.indexOf("memory")>0){
        sMemory = String(cMQTTPayload);     
    }else if(sTopic.indexOf("swap")>0){
        sSwap = String(cMQTTPayload);     
    }else if(sTopic.indexOf("uptime")>0){
        sUptimeHours = String(cMQTTPayload);     
    }else if(sTopic.indexOf("sys")>0){
        sClockSpeed = String(cMQTTPayload);     
    }        
}

bool ESPJarvis::connectMQTTBroker(String sClientName, String sClientPassword, String sClientID, String sServer, int iPort){
    client.setClient(esp32);
    client.setServer(sServer.c_str(), iPort);
    client.setCallback(std::bind(&ESPJarvis::MQTTCallbackFunction, this,  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    if (client.connect(sClientID.c_str(), sClientName.c_str(), sClientPassword.c_str())) {
         client.subscribe((_sMqttTopicPrefix + "/cpuload").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/diskusage").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/gpu_freq").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/memory").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/swap").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/uptime_hours").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/sys_clock_speed").c_str(),iBrokerQoS);
         return true;
     } else {
         return false;
     }
}

void ESPJarvis::run(){
   client.loop();; 
}

bool ESPJarvis::connect(){
   return connectMQTTBroker(this->sBrokerClientName, this->sBrokerClientPassword, this->sBrokerClientID, this->sBrokerServer, this->iBrokerPort); 
}

int ESPJarvis::getServerState(){
    return client.state();
}

// x,y == coords of centre of arc
// start_angle = 0 - 359
// seg_count = number of inc * degree segments to draw
// rx = x axis radius
// yx = y axis radius
// w  = width (thickness) of arc in pixels
// colour = 16 bit colour value
// Note if rx and ry are the same then an arc of a circle is drawn
void ESPJarvis::drawArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, int colour, ScreenType type)
{
    byte seg = 1;
    byte inc = 1;
    // Calculate first pair of coordinates for segment start
    float sx = cos((start_angle - 90) * DEG_TO_RAD);
    float sy = sin((start_angle - 90) * DEG_TO_RAD);
    uint16_t x0 = sx * (rx - w) + x;
    uint16_t y0 = sy * (ry - w) + y;
    uint16_t x1 = sx * rx + x;
    uint16_t y1 = sy * ry + y;
    // Draw colour blocks every inc degrees
    for (int i = start_angle; i < start_angle + seg * seg_count; i += inc) {
        // Calculate pair of coordinates for segment end
        float sx2 = cos((i + seg - 90) * DEG_TO_RAD);
        float sy2 = sin((i + seg - 90) * DEG_TO_RAD);
        int x2 = sx2 * (rx - w) + x;
        int y2 = sy2 * (ry - w) + y;
        int x3 = sx2 * rx + x;
        int y3 = sy2 * ry + y;
    if(_bUseSSD1306&&type==SSD1306){
        _screen_ssd1306->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
        _screen_ssd1306->fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    }
    if(_bUseST7735&&type==ST7735){
        _screen_st7735->fillTriangle(x0, y0, x1, y1, x2, y2, colour);
        _screen_st7735->fillTriangle(x1, y1, x2, y2, x3, y3, colour);
    }
    // Copy segment end to sgement start for next segment
    x0 = x2;
    y0 = y2;
    x1 = x3;
    y1 = y3;
  }
}
void ESPJarvis::setMaxClockSpeed(int MaxClockSpeed)
{
    if(MaxClockSpeed > 0){
        _iMaxClockSpeed = MaxClockSpeed;
    }
}
void ESPJarvis::setMaxGPUFreq(int MaxGPUFreq)
{
    if(MaxGPUFreq > 0){
        _iMaxGPUFreq = MaxGPUFreq;
    }
}
void ESPJarvis::showCPUGPUPage(ScreenType type){
    int iGPUFreq=sGpuFreq.toInt();
    int iCPUTemp=sCpuTemp.toInt();
    int iClockSpeed=sClockSpeed.toInt();
    float iCpuLoad=sCpuLoad.toFloat();
    int iGPUDegree = (int)(iGPUFreq*360/_iMaxGPUFreq);
    if(iGPUDegree>360) iGPUDegree=360;
    int iCPUDegree = (int)(iClockSpeed*360/_iMaxClockSpeed);
    if(iCPUDegree>360) iCPUDegree=360;
    if(_bUseSSD1306&&type==SSD1306){
         _screen_ssd1306->clearDisplay();
        drawArc(32, 32, 0, iGPUDegree, 32, 32, 4, WHITE);
        drawArc(32, 32, iGPUDegree, 360, 28, 28, 1, WHITE);
        _screen_ssd1306->setCursor(24,10);
        _screen_ssd1306->print("GPU");
        if(iGPUFreq!=0){
            if(iGPUFreq>=100){
                _screen_ssd1306->setCursor(13,30);
            }
            else{
                _screen_ssd1306->setCursor(15,30);
            }
            _screen_ssd1306->print(sGpuFreq+" Mhz");
        }
        if(iCPUTemp!=0){
            _screen_ssd1306->setCursor(18,40);
            _screen_ssd1306->print(String(iCPUTemp)+" C");
        }

        drawArc(96, 32, 0, iCPUDegree, 32, 32, 4, WHITE);
        drawArc(96, 32, iCPUDegree, 360, 28, 28, 1, WHITE);
        _screen_ssd1306->setCursor(88,10);
        _screen_ssd1306->print("CPU");
        if(iClockSpeed!=0){
            if(iClockSpeed>=1000){
                _screen_ssd1306->setCursor(72,30);
            }
            else{
                _screen_ssd1306->setCursor(75,30);
            }
            _screen_ssd1306->print(sClockSpeed+" Mhz");
        }
        if(iCpuLoad!=0){
            if(iCpuLoad>=10){
                _screen_ssd1306->setCursor(82,20);
            }
            else{
                _screen_ssd1306->setCursor(85,20);
            }
            _screen_ssd1306->print(sCpuLoad+" %");
        }
        if(iCPUTemp!=0){
            _screen_ssd1306->setCursor(85,40);
            _screen_ssd1306->print(String(iCPUTemp)+" C");
        }
        
        _screen_ssd1306->display();
    }
    if(_bUseST7735&&type==ST7735){

    }
   
}
void ESPJarvis::showGPUPage(ScreenType type){
    int iGPUFreq=sGpuFreq.toInt();
    int iCPUTemp=sCpuTemp.toInt();
    int iGPUDegree = (int)(iGPUFreq*360/_iMaxGPUFreq);
    if(_bUseSSD1306&&type==SSD1306){
        if(iGPUDegree>360) iGPUDegree=360;
        _screen_ssd1306->clearDisplay();
        _screen_ssd1306->drawBitmap(64, 0, bitmapGPU64, 64, 64, WHITE);
        drawArc(32, 32, 0, iGPUDegree, 32, 32, 4, WHITE);
        drawArc(32, 32, iGPUDegree, 360, 28, 28, 1, WHITE);
        _screen_ssd1306->setCursor(24,10);
        _screen_ssd1306->print("GPU");
        if(iGPUFreq!=0){
            if(iGPUFreq>=100){
                _screen_ssd1306->setCursor(13,30);
            }
            else{
                _screen_ssd1306->setCursor(15,30);
            }
            _screen_ssd1306->print(sGpuFreq+" Mhz");
        }
        if(iCPUTemp!=0){
            _screen_ssd1306->setCursor(18,40);
            _screen_ssd1306->print(String(iCPUTemp)+" C");
        }
        _screen_ssd1306->display();
    }
    if(_bUseST7735&&type==ST7735){

    }
}
void ESPJarvis::ssd1306_drawCirclewithIcon(const uint8_t bitmap[],int degree, bool icon_left){
    if(_bUseSSD1306){
        if(icon_left){
            _screen_ssd1306->drawBitmap(0, 0, bitmap, 64, 64, SSD1306_WHITE);
            drawCircleDegree(96,32,degree,0,SSD1306_WHITE,SSD1306_BLACK,4,32,SSD1306);
            drawCircleDegree(96,32,360,0,SSD1306_WHITE,SSD1306_BLACK,1,28,SSD1306);
        }        
    }

}
void ESPJarvis::showCPUPage(ScreenType type){
    int iClockSpeed=sClockSpeed.toInt();
    float fCpuLoad=sCpuLoad.toFloat();
    int iCPUTemp=sCpuTemp.toInt();
    int iCPUDegree = (int)(iClockSpeed*360/_iMaxClockSpeed);
    if(_bUseSSD1306&&type==SSD1306){
        if(iCPUDegree>360) iCPUDegree=360;
        _screen_ssd1306->clearDisplay();
        ssd1306_drawCirclewithIcon(bitmapChip64,iCPUDegree,true);
        _screen_ssd1306->setCursor(88,10);
        _screen_ssd1306->print("CPU");
        if(iClockSpeed!=0){
            if(iClockSpeed>=1000){
                _screen_ssd1306->setCursor(72,30);
            }
            else{
                _screen_ssd1306->setCursor(75,30);
            }
            _screen_ssd1306->print(sClockSpeed+" Mhz");
        }
        if(fCpuLoad!=0){
            if(fCpuLoad>=10){
                _screen_ssd1306->setCursor(82,20);
            }
            else{
                _screen_ssd1306->setCursor(85,20);
            }
            _screen_ssd1306->print(sCpuLoad+" %");
        }
        if(iCPUTemp!=0){
            _screen_ssd1306->setCursor(85,40);
            _screen_ssd1306->print(String(iCPUTemp)+" C");
        }
        _screen_ssd1306->display();
    }
    if(_bUseST7735&&type==ST7735){
        //drawArc(96, 32, 0, 360, 16, 16, 4, ST77XX_MAGENTA, ST7735);
        //drawArc(96, 32, 0, iCPUDegree, 16, 16, 4, ST77XX_BLUE, ST7735);
        //drawArc(96, 32, iCPUDegree, 360, 10, 10, 3, ST77XX_RED, ST7735);
        //delay(1000);
        drawCircleDegree(20, 20, iCPUDegree);
        drawCircleDegree(60, 20, iCPUDegree);
        drawCircleDegree(100, 20, iCPUDegree);
        drawCircleDegree(20, 60, iCPUDegree-10);
        drawCircleDegree(60, 60, iCPUDegree-10);
        drawCircleDegree(100, 60, iCPUDegree-10);
        drawCircleDegree(20, 100, iCPUDegree-20);
        drawCircleDegree(60, 100, iCPUDegree-20);
        drawCircleDegree(100, 100, iCPUDegree-20);

    }
}
void ESPJarvis::showMemoryPage(ScreenType type){
    int iMemory=sMemory.toInt();
    int iDiskUsage=sDiskUsage.toInt();
    int iSwap=sSwap.toInt();
    int iHours=sUptimeHours.toInt(); 
    if(_bUseSSD1306&&type==SSD1306){
        _screen_ssd1306->clearDisplay();
        if(iSwap!=0){
            _screen_ssd1306->drawBitmap(0, 0, bitmapSwap32, 32, 32, WHITE);
            _screen_ssd1306->drawRect(35,3,45,10, WHITE);
            _screen_ssd1306->fillRect(35,3,iSwap*45/100,10, WHITE);
            _screen_ssd1306->setCursor(81,3);
            _screen_ssd1306->print(String(iSwap)+"%");
        }
        if(iMemory!=0){
            _screen_ssd1306->drawBitmap(96, 0, bitmapRAM32, 32, 32, WHITE);
            _screen_ssd1306->drawRect(50,18,50,10, WHITE);
            _screen_ssd1306->fillRect(100-50*iMemory/100,18,50*iMemory/100,10, WHITE);
            _screen_ssd1306->setCursor(32,18);
            _screen_ssd1306->print(String(iMemory)+"%");
        }
        if(iDiskUsage!=0){
            _screen_ssd1306->drawRect(35,33,45,10, WHITE);
            _screen_ssd1306->fillRect(35,33,iDiskUsage*45/100,10, WHITE);
            _screen_ssd1306->setCursor(82,37);
            _screen_ssd1306->print(String(iDiskUsage)+"%");
        }
        if(iHours!=0){
            _screen_ssd1306->drawBitmap(0, 32, bitmapHDD32, 32, 32, WHITE);
            _screen_ssd1306->setCursor(50,48);
            _screen_ssd1306->print("Run Time:");   
            if(iHours==(int)1){
                _screen_ssd1306->setCursor(60,57);
                _screen_ssd1306->print("1 Hours");        
            }else{
                _screen_ssd1306->setCursor(60,57);
                _screen_ssd1306->print(String(iHours)+" Hours");
            }
        }
        _screen_ssd1306->display();
    }    
}
void ESPJarvis::showVersion(ScreenType type){
    if(_bUseSSD1306&&type==SSD1306){
        _screen_ssd1306->clearDisplay();
        _screen_ssd1306->drawBitmap(47, 0, bitmapTiger64, 64, 64, WHITE);
        _screen_ssd1306->setCursor(20,20);
        _screen_ssd1306->print(SOFTWARE_VERSION);
        _screen_ssd1306->setCursor(20,40);
        _screen_ssd1306->print("2022");
        _screen_ssd1306->display();
        delay(3000);
        _screen_ssd1306->clearDisplay();    
    }
    if(_bUseST7735&&type==ST7735){
        _screen_st7735->fillScreen(ST77XX_MAGENTA);
        _screen_st7735->drawBitmap(47, 0, bitmapTiger64, 64, 64, ST77XX_RED);
        _screen_st7735->setCursor(20,20);
        _screen_st7735->print(SOFTWARE_VERSION);
        _screen_st7735->setCursor(20,40);
        _screen_st7735->print("2022");
        delay(3000);
        _screen_st7735->fillScreen(ST77XX_MAGENTA);   

    }
}

void ESPJarvis::printMSG(int lineNumber, const char* text, ScreenType type){
  int i=0;
  int textLength=((String)text).length();
  int base=SCREEN_TEXT_PER_LINE*(lineNumber-1);
  if(textLength>SCREEN_TEXT_PER_LINE) textLength = SCREEN_TEXT_PER_LINE;
  for(i=0;i<textLength;i++){
    cScreenBuffer[base+i]=text[i];    
  }
  for(i=textLength;i<21;i++){
    cScreenBuffer[base+i]=' ';    
  }
  cScreenBuffer[SCREEN_TEXT_PER_LINE*SCREEN_LINE_OF_TEXT]=0;
  if(_bUseSSD1306&&type==SSD1306){
    _screen_ssd1306->clearDisplay();
    _screen_ssd1306->setCursor(0,0);
    _screen_ssd1306->println(cScreenBuffer);
    _screen_ssd1306->display();  
  }
  if(_bUseST7735&&type==ST7735){
    _screen_st7735->fillScreen(ST77XX_MAGENTA);
    _screen_st7735->setCursor(0,0);
    _screen_st7735->println(cScreenBuffer);
  }
}

void ESPJarvis::drawCircleDegree(int x, int y, int degree , int degreeOld, int color , int colorBackground, int width , int radius , ScreenType type){
    if(degreeOld == 0){//draw complete Circle
        if(_bUseSSD1306&&type==SSD1306){
            drawArc(x, y, 0, degree, radius, radius, width, color, SSD1306);
            drawArc(x, y, degree, 359 - degree , radius, radius, width, colorBackground, SSD1306);  
        }
        if(_bUseST7735&&type==ST7735){
            drawArc(x, y, 0, degree, radius, radius, width, color, ST7735);
            drawArc(x, y, degree, 359 - degree , radius, radius, width, colorBackground, ST7735);  
        }
    }
    else if (degree == degreeOld){
        return;
    }
    else if(degree < degreeOld){
        if(_bUseSSD1306&&type==SSD1306){
            drawArc(x, y, degree, degreeOld - degree , radius, radius, width, colorBackground, SSD1306); 
        }
        if(_bUseST7735&&type==ST7735){
            drawArc(x, y, degree, degreeOld - degree , radius, radius, width, colorBackground, ST7735);   
        }     
    }
    else{
        if(_bUseSSD1306&&type==SSD1306){
            drawArc(x, y, degreeOld, degree - degreeOld , radius, radius, width, color, SSD1306); 
        }
        if(_bUseST7735&&type==ST7735){
            drawArc(x, y, degreeOld, degree - degreeOld , radius, radius, width, color, ST7735); 
        }   
    }        
}

void ESPJarvis::initScreenBuffer(){
  for(int i=0;i<SCREEN_TEXT_PER_LINE*SCREEN_LINE_OF_TEXT+1;i++){
    cScreenBuffer[i]=' ';    
  }
  cScreenBuffer[SCREEN_TEXT_PER_LINE*SCREEN_LINE_OF_TEXT]=0;  
}

void ESPJarvis::setMqttTopicPrefix(String Prefix){
    _sMqttTopicPrefix = Prefix;
}
String ESPJarvis::getMqttTopicPrefix(){
    return _sMqttTopicPrefix;
}

