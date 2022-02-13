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
    _screen = &screen;
    sBrokerServer = sServer;
    iBrokerPort = iPort;
    initScreenBuffer();    
}

void ESPJarvis::attachScreen(Adafruit_SSD1306 &screen){
    _screen = &screen;
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
// seg_count = number of 3 degree segments to draw (120 => 360 degree arc)
// rx = x axis radius
// yx = y axis radius
// w  = width (thickness) of arc in pixels
// colour = 16 bit colour value
// Note if rx and ry are the same then an arc of a circle is drawn
void ESPJarvis::drawArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, unsigned int colour)
{

  byte seg = 1; // Segments are 3 degrees wide = 120 segments for 360 degrees
  byte inc = 1; // Draw segments every 3 degrees, increase to 6 for segmented ring

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


    _screen->fillTriangle(x0, y0, x1, y1, x2, y2, WHITE);
    _screen->fillTriangle(x1, y1, x2, y2, x3, y3, WHITE);

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
void ESPJarvis::showCPUGPUPage(){
    int iGPUFreq=sGpuFreq.toInt();
    int iCPUTemp=sCpuTemp.toInt();
    int iClockSpeed=sClockSpeed.toInt();
    float iCpuLoad=sCpuLoad.toFloat();
    int iGPUDegree = (int)(iGPUFreq*360/_iMaxGPUFreq);
    if(iGPUDegree>360) iGPUDegree=360;
    int iCPUDegree = (int)(iClockSpeed*360/_iMaxClockSpeed);
    if(iCPUDegree>360) iCPUDegree=360;
    _screen->clearDisplay();
    drawArc(32, 32, 0, iGPUDegree, 32, 32, 4, WHITE);
    drawArc(32, 32, iGPUDegree, 360, 28, 28, 1, WHITE);
    _screen->setCursor(24,10);
    _screen->print("GPU");
    if(iGPUFreq!=0){
        if(iGPUFreq>=100){
            _screen->setCursor(13,30);
        }
        else{
            _screen->setCursor(15,30);
        }
        _screen->print(sGpuFreq+" Mhz");
    }
    if(iCPUTemp!=0){
        _screen->setCursor(18,40);
        _screen->print(String(iCPUTemp)+" C");
    }

    drawArc(96, 32, 0, iCPUDegree, 32, 32, 4, WHITE);
    drawArc(96, 32, iCPUDegree, 360, 28, 28, 1, WHITE);
    _screen->setCursor(88,10);
    _screen->print("CPU");
    if(iClockSpeed!=0){
        if(iClockSpeed>=1000){
            _screen->setCursor(72,30);
        }
        else{
            _screen->setCursor(75,30);
        }
        _screen->print(sClockSpeed+" Mhz");
    }
    if(iCpuLoad!=0){
        if(iCpuLoad>=10){
            _screen->setCursor(82,20);
        }
        else{
            _screen->setCursor(85,20);
        }
        _screen->print(sCpuLoad+" %");
    }
    if(iCPUTemp!=0){
        _screen->setCursor(85,40);
        _screen->print(String(iCPUTemp)+" C");
    }
    
    _screen->display();
}
void ESPJarvis::showGPUPage(){
    int iGPUFreq=sGpuFreq.toInt();
    int iCPUTemp=sCpuTemp.toInt();
    int iGPUDegree = (int)(iGPUFreq*360/_iMaxGPUFreq);
    if(iGPUDegree>360) iGPUDegree=360;
    _screen->clearDisplay();
    _screen->drawBitmap(64, 0, bitmapGPU64, 64, 64, WHITE);
    drawArc(32, 32, 0, iGPUDegree, 32, 32, 4, WHITE);
    drawArc(32, 32, iGPUDegree, 360, 28, 28, 1, WHITE);
    _screen->setCursor(24,10);
    _screen->print("GPU");
    if(iGPUFreq!=0){
        if(iGPUFreq>=100){
            _screen->setCursor(13,30);
        }
        else{
            _screen->setCursor(15,30);
        }
        _screen->print(sGpuFreq+" Mhz");
    }
    if(iCPUTemp!=0){
        _screen->setCursor(18,40);
        _screen->print(String(iCPUTemp)+" C");
    }
    _screen->display();
}
void ESPJarvis::showCPUPage(){
    int iClockSpeed=sClockSpeed.toInt();
    float iCpuLoad=sCpuLoad.toFloat();
    int iCPUTemp=sCpuTemp.toInt();
    int iCPUDegree = (int)(iClockSpeed*360/_iMaxClockSpeed);
    if(iCPUDegree>360) iCPUDegree=360;
    _screen->clearDisplay();
    _screen->drawBitmap(0, 0, bitmapChip64, 64, 64, WHITE);
    drawArc(96, 32, 0, iCPUDegree, 32, 32, 4, WHITE);
    drawArc(96, 32, iCPUDegree, 360, 28, 28, 1, WHITE);
    _screen->setCursor(88,10);
    _screen->print("CPU");
    if(iClockSpeed!=0){
        if(iClockSpeed>=1000){
            _screen->setCursor(72,30);
        }
        else{
            _screen->setCursor(75,30);
        }
        _screen->print(sClockSpeed+" Mhz");
    }
    if(iCpuLoad!=0){
        if(iCpuLoad>=10){
            _screen->setCursor(82,20);
        }
        else{
            _screen->setCursor(85,20);
        }
        _screen->print(sCpuLoad+" %");
    }
    if(iCPUTemp!=0){
        _screen->setCursor(85,40);
        _screen->print(String(iCPUTemp)+" C");
    }
    _screen->display();
}
void ESPJarvis::showMemoryPage(){
    int iMemory=sMemory.toInt();
    int iDiskUsage=sDiskUsage.toInt();
    int iSwap=sSwap.toInt();
    int iHours=sUptimeHours.toInt(); 
    _screen->clearDisplay();
    if(iSwap!=0){
        _screen->drawBitmap(0, 0, bitmapSwap32, 32, 32, WHITE);
        _screen->drawRect(35,3,45,10, WHITE);
        _screen->fillRect(35,3,iSwap*45/100,10, WHITE);
        _screen->setCursor(81,3);
        _screen->print(String(iSwap)+"%");
    }
    if(iMemory!=0){
        _screen->drawBitmap(96, 0, bitmapRAM32, 32, 32, WHITE);
        _screen->drawRect(50,18,50,10, WHITE);
        _screen->fillRect(100-50*iMemory/100,18,50*iMemory/100,10, WHITE);
        _screen->setCursor(32,18);
        _screen->print(String(iMemory)+"%");
    }
    if(iDiskUsage!=0){
        _screen->drawRect(35,33,45,10, WHITE);
        _screen->fillRect(35,33,iDiskUsage*45/100,10, WHITE);
        _screen->setCursor(82,37);
        _screen->print(String(iDiskUsage)+"%");
    }
    if(iHours!=0){
        _screen->drawBitmap(0, 32, bitmapHDD32, 32, 32, WHITE);
        _screen->setCursor(50,48);
        _screen->print("Run Time:");   
        if(iHours==(int)1){
            _screen->setCursor(60,57);
            _screen->print("1 Hours");        
        }else{
            _screen->setCursor(60,57);
            _screen->print(String(iHours)+" Hours");
        }
    }
    _screen->display();
    
}
void ESPJarvis::showVersion(){
    _screen->clearDisplay();
    _screen->drawBitmap(47, 0, bitmapTiger64, 64, 64, WHITE);
    _screen->setCursor(20,20);
    _screen->print(SOFTWARE_VERSION);
    _screen->setCursor(20,40);
    _screen->print("2022");
    _screen->display();
    delay(3000);
    _screen->clearDisplay();    
}

void ESPJarvis::printMSG(int lineNumber, const char* text){
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
  _screen->clearDisplay();
  _screen->setCursor(0,0);
  _screen->println(cScreenBuffer);
  _screen->display();  
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

