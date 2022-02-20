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
    _screen_st7735->fillScreen(_iBackgroundColour);
}

void ESPJarvis::setClientData(String sClientID, String sClientName, String sClientPassword){
    sBrokerClientID = sClientID;
    sBrokerClientName = sClientName;
    sBrokerClientPassword = sClientPassword;
}

void ESPJarvis::MQTTCallbackFunction(char *topic, byte *payload, unsigned int length) {
    String sTopic = String(topic);
    String tmp;
    for (int i = 0; i < length; i++) {
        cMQTTPayload[i]= (char) payload[i];
    }
    cMQTTPayload[length] = 0;
    // if(sTopic.indexOf("cpuload")>0){
    //     sCpuLoad = String(cMQTTPayload);        
    // }else if(sTopic.indexOf("temp")>0){
    //     sCpuTemp = String(cMQTTPayload);     
    // }else if(sTopic.indexOf("disk")>0){
    //     sDiskUsage = String(cMQTTPayload);     
    // }else if(sTopic.indexOf("gpu")>0){
    //     sGpuFreq = String(cMQTTPayload);     
    // }else if(sTopic.indexOf("memory")>0){
    //     sMemory = String(cMQTTPayload);     
    // }else if(sTopic.indexOf("swap")>0){
    //     sSwap = String(cMQTTPayload);     
    // }else if(sTopic.indexOf("uptime")>0){
    //     sUptimeHours = String(cMQTTPayload);     
    // }else if(sTopic.indexOf("sys")>0){
    //     sClockSpeed = String(cMQTTPayload);     
    // }
    // if(sTopic.indexOf("d_1")>0){
    //     _iCpuLoad[1] = String(cMQTTPayload).toInt();   
    // }else if(sTopic.indexOf("d_2")>0){
    //     _iCpuLoad[2] = String(cMQTTPayload).toInt();  
    // }else if(sTopic.indexOf("d_3")>0){
    //     _iCpuLoad[3] = String(cMQTTPayload).toInt();  
    // }else if(sTopic.indexOf("d_4")>0){
    //     _iCpuLoad[4] = String(cMQTTPayload).toInt();  
    // }else if(sTopic.indexOf("d_5")>0){
    //     _iCpuLoad[5] = String(cMQTTPayload).toInt();  
    // }else if(sTopic.indexOf("d_6")>0){
    //     _iCpuLoad[6] = String(cMQTTPayload).toInt();  
    // } 
    if(sTopic.indexOf("k_1")>0){
        _iCpuClock[1] = String(cMQTTPayload).toInt();   
    }else if(sTopic.indexOf("k_2")>0){
        _iCpuClock[2] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("k_3")>0){
        _iCpuClock[3] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("k_4")>0){
        _iCpuClock[4] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("k_5")>0){
        _iCpuClock[5] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("k_6")>0){
        _iCpuClock[6] = String(cMQTTPayload).toInt(); 
    }else if(sTopic.indexOf("p_1")>0){
        _iCpuTemp[1] = String(cMQTTPayload).toInt();   
    }else if(sTopic.indexOf("p_2")>0){
        _iCpuTemp[2] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("p_3")>0){
        _iCpuTemp[3] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("p_4")>0){
        _iCpuTemp[4] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("p_5")>0){
        _iCpuTemp[5] = String(cMQTTPayload).toInt();  
    }else if(sTopic.indexOf("p_6")>0){
        _iCpuTemp[6] = String(cMQTTPayload).toInt(); 
    }
    // for(int i=1;i<7;i++){
    //     tmp=String("k_")+String(i);
    //     if(sTopic.indexOf(tmp)>0){
    //         printMSG(1,topic);
    //         printMSG(2,cMQTTPayload);
    //         _iCpuClock[i] = int(cMQTTPayload); 
    //         break;
    //     }
    // }

}

bool ESPJarvis::connectMQTTBroker(String sClientName, String sClientPassword, String sClientID, String sServer, int iPort){
    client.setClient(esp32);
    client.setServer(sServer.c_str(), iPort);
    client.setCallback(std::bind(&ESPJarvis::MQTTCallbackFunction, this,  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
    if (client.connect(sClientID.c_str(), sClientName.c_str(), sClientPassword.c_str())) {
         client.subscribe((_sMqttTopicPrefix + "/cpuload").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuload_1").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuload_2").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuload_3").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuload_4").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuload_5").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuload_6").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp_1").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp_2").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp_3").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp_4").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp_5").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cputemp_6").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/diskusage").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/gpu_freq").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/memory").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/swap").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/uptime_hours").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/sys_clock_speed").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuclock_1").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuclock_2").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuclock_3").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuclock_4").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuclock_5").c_str(),iBrokerQoS);
         client.subscribe((_sMqttTopicPrefix + "/cpuclock_6").c_str(),iBrokerQoS);
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

void ESPJarvis::setBackgroundColour(int colour){
    _iBackgroundColour = colour;
}


void ESPJarvis::drawCpuCircle(int x,  int y, int coreIndex, int color, ScreenType type){
    if(_bUseSSD1306&&type==SSD1306){
         if(_iCpuClock[coreIndex] != 0){
            drawCircleDegree(x, y,  (int)(_iCpuClock[coreIndex]*360/_iMaxClockSpeed), 0, color, SSD1306_BLACK, 4, 20, SSD1306);
            _screen_ssd1306->fillCircle(x, y, 15, SSD1306_BLACK);
            if(_iCpuClock[coreIndex]<1000){
                _screen_ssd1306->setCursor(x-8, y-4);
            } else {
                _screen_ssd1306->setCursor(x-12, y-4);
            }
            _screen_ssd1306->print(_iCpuClock[coreIndex]);
            _screen_ssd1306->setCursor(x-6, y+5);
            _screen_ssd1306->print(_iCpuTemp[coreIndex]);
         }
    }
    if(_bUseST7735&&type==ST7735){
        if(_iCpuClock[coreIndex] != 0){
            drawCircleDegree(x, y,  (int)(_iCpuClock[coreIndex]*360/_iMaxClockSpeed), (int)(_iCpuClockOld[coreIndex]*360/_iMaxClockSpeed), color, ST77XX_GRAY);
            //_screen_st7735->fillCircle(x, y, 15, _iBackgroundColour);
            _screen_st7735->setTextColor(ST77XX_GRAY);
            _screen_st7735->setCursor(x-2, y-13);
            _screen_st7735->print(coreIndex);
            if(_iCpuClock[coreIndex] != _iCpuClockOld[coreIndex]){
                _screen_st7735->fillRect(x-12, y-4, 25, 7, _iBackgroundColour);
                if(_iCpuClock[coreIndex]<1000){
                    _screen_st7735->setCursor(x-8, y-4);
                } else {
                    _screen_st7735->setCursor(x-12, y-4);
                }
                if(_iBackgroundColour == ST77XX_BLACK){
                    _screen_st7735->setTextColor(ST77XX_GRAY);
                }else{
                    _screen_st7735->setTextColor(ST77XX_RED);
                }
                _screen_st7735->print(_iCpuClock[coreIndex]);
            }
            if(_iCpuTemp[coreIndex] != _iCpuTempOld[coreIndex]){
                _screen_st7735->fillRect(x-6, y+4, 11, 9, _iBackgroundColour);
                _screen_st7735->setCursor(x-6, y+5);
                if(_iCpuTemp[coreIndex]<75){
                    if(_iBackgroundColour == ST77XX_BLACK){
                        _screen_st7735->setTextColor(ST77XX_GREEN);
                    }else{
                        _screen_st7735->setTextColor(ST77XX_YELLOW);
                    }
                }else{
                    _screen_st7735->setTextColor(ST77XX_RED);
                }
                _screen_st7735->print(_iCpuTemp[coreIndex]);
            }
            _iCpuClockOld[coreIndex] = _iCpuClock[coreIndex];
            _iCpuTempOld[coreIndex] = _iCpuTempOld[coreIndex];

        }
    }
    
}
void ESPJarvis::setDisplayMode(DisplayMode Mode){
    if(_bUseST7735){
        switch (Mode)
        {
        case DayMode:
            setBackgroundColour(ST77XX_MAGENTA);
            _screen_st7735->fillScreen(_iBackgroundColour);
            for(int i=1; i<NUM_OF_CORES+1; i++){
                _iCpuClockOld[i] = 0;
                _iCpuTemp[i] = 0;
            }
            _displayMode = DayMode;
            break;
        case DarkMode:
            setBackgroundColour(ST77XX_BLACK);
            _screen_st7735->fillScreen(_iBackgroundColour);
            for(int i=1; i<NUM_OF_CORES+1; i++){
                _iCpuClockOld[i] = 0;
                _iCpuTemp[i] = 0;
            }
            _displayMode = DarkMode;
            break;
        default:
            break;
        }
    }

}
void ESPJarvis::showSixCoreCpu(String cpuName, ScreenType type, HardwareType hardware){
    if(_bUseST7735&&type==ST7735){
        if(_iCpuClockOld[1] ==0 ){
            _screen_st7735->fillScreen(_iBackgroundColour);
            if(_iBackgroundColour == ST77XX_BLACK){
                _screen_st7735->drawBitmap(1, 83, bitmapChip64, 64, 64, ST77XX_GRAY);
                _screen_st7735->drawBitmap(65, 83, bitmapTiger64, 64, 64, ST77XX_RED);
            } else{
                _screen_st7735->drawBitmap(1, 83, bitmapChip64, 64, 64, ST77XX_YELLOW);
                _screen_st7735->drawBitmap(65, 83, bitmapTiger64, 64, 64, ST77XX_BLUE);
            }
            // if( hardware == Intel){
            //     _screen_st7735->drawBitmap(9, 93, bitmapIntel48, 48, 48, ST77XX_BLUE);
            // } 
            switch (hardware)
            {
            case Intel:
                 _screen_st7735->drawBitmap(9, 93, bitmapIntel48, 48, 48, ST77XX_BLUE);
                break;
            case AMD:
                _screen_st7735->drawBitmap(9, 88, bitmapAMD50, 50, 50, ST77XX_RED);
                break;
            case AMD_RYZEN:
                _screen_st7735->drawBitmap(9, 88, bitmapAMD50, 50, 50, ST77XX_RED);
                if(_iBackgroundColour == ST77XX_BLACK){
                    _screen_st7735->drawBitmap(25, 85, bitmapRyzen24, 24, 24, ST77XX_GRAY);
                }else{
                    _screen_st7735->drawBitmap(25, 85, bitmapRyzen24, 24, 24, ST77XX_YELLOW);
                }
                break;            
            default:
                break;
            }
            _screen_st7735->setCursor(1, 150);
            _screen_st7735->setTextColor(ST77XX_RED);
            _screen_st7735->print(cpuName);
            switch (_displayMode)
            {
            case DayMode:
                drawCpuCircle(22, 20, 1, ST77XX_BLUE, ST7735);
                drawCpuCircle(64, 20, 2, ST77XX_BLUE, ST7735);
                drawCpuCircle(107, 20, 3, ST77XX_BLUE, ST7735);
                drawCpuCircle(22, 62, 4, ST77XX_BLUE, ST7735);
                drawCpuCircle(64, 62, 5, ST77XX_BLUE, ST7735);
                drawCpuCircle(107, 62, 6, ST77XX_BLUE, ST7735);
                break;
            case DarkMode:
                drawCpuCircle(22, 20, 1, ST77XX_RED, ST7735);
                drawCpuCircle(64, 20, 2, ST77XX_RED, ST7735);
                drawCpuCircle(107, 20, 3, ST77XX_RED, ST7735);
                drawCpuCircle(22, 62, 4, ST77XX_RED, ST7735);
                drawCpuCircle(64, 62, 5, ST77XX_RED, ST7735);
                drawCpuCircle(107, 62, 6, ST77XX_RED, ST7735);
                break;            
            default:
                break;
            }
        }else{
            switch (_displayMode)
            {
            case DayMode:
                drawCpuCircle(22, 20, 1, ST77XX_BLUE, ST7735);
                drawCpuCircle(64, 20, 2, ST77XX_BLUE, ST7735);
                drawCpuCircle(107, 20, 3, ST77XX_BLUE, ST7735);
                drawCpuCircle(22, 62, 4, ST77XX_BLUE, ST7735);
                drawCpuCircle(64, 62, 5, ST77XX_BLUE, ST7735);
                drawCpuCircle(107, 62, 6, ST77XX_BLUE, ST7735);
                break;
            case DarkMode:
                drawCpuCircle(22, 20, 1, ST77XX_RED, ST7735);
                drawCpuCircle(64, 20, 2, ST77XX_RED, ST7735);
                drawCpuCircle(107, 20, 3, ST77XX_RED, ST7735);
                drawCpuCircle(22, 62, 4, ST77XX_RED, ST7735);
                drawCpuCircle(64, 62, 5, ST77XX_RED, ST7735);
                drawCpuCircle(107, 62, 6, ST77XX_RED, ST7735);
                break;            
            default:
                break;
            }
        }
    }
}
void ESPJarvis::showCPUPage(ScreenType type){
    if(_bUseSSD1306&&type==SSD1306){
        int iClockSpeed=sClockSpeed.toInt();
        float fCpuLoad=sCpuLoad.toFloat();
        int iCPUTemp=sCpuTemp.toInt();
        int iCPUDegree = (int)(iClockSpeed*360/_iMaxClockSpeed);
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
        _screen_st7735->fillScreen(_iBackgroundColour);
        _screen_st7735->drawBitmap(47, 0, bitmapTiger64, 64, 64, ST77XX_RED);
        _screen_st7735->setCursor(20,20);
        _screen_st7735->print(SOFTWARE_VERSION);
        _screen_st7735->setCursor(20,40);
        _screen_st7735->print("2022");
        delay(3000);
        _screen_st7735->fillScreen(_iBackgroundColour);   

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
    _screen_st7735->fillScreen(_iBackgroundColour);
    _screen_st7735->setCursor(0,0);
    _screen_st7735->println(cScreenBuffer);
  }
}

void ESPJarvis::drawCircleDegree(int x, int y, int degree , int degreeOld, int color , int colorBackground, int width , int radius , ScreenType type){
    if (degree < 0){
        degree = 0;
    }else if(degree >= 359){
        degree = 359;
    }
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

