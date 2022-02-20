#ifndef _ESPJARVIS_H 
#define _ESPJARVIS_H 

#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>
#include <HTTPClient.h> 
#include <WiFiClient.h>
#include <PubSubClient.h> 
#include <Adafruit_GFX.h>   
#include <Adafruit_SSD1306.h>
#include <Adafruit_ST7735.h>

#define SOFTWARE_VERSION "V1.0"
#define SCREEN_TEXT_PER_LINE 21//每行显示多少字符
#define SCREEN_LINE_OF_TEXT 8//显示多少行字符
#define ST77XX_GRAY 0x8410

#define NUM_OF_CORES 20

enum ScreenType {
	SSD1306,
	ST7735
};

enum HardwareType {
	Intel,
	AMD,
	AMD_RYZEN
};

enum DisplayMode {
	DarkMode,
	DayMode
};

class ESPJarvis {
    public:
        ESPJarvis(String sServer, int iPort = 1883);
        ESPJarvis(Adafruit_SSD1306 &screen, String sServer, int iPort = 1883);
		ESPJarvis(Adafruit_ST7735 &screen, String sServer, int iPort = 1883);
        void attachScreen(Adafruit_SSD1306 &screen);
		void attachScreen(Adafruit_ST7735 &screen);
        void setClientData(String sClientID, String sClientName, String sClientPassword);
        bool connect();
        int getServerState();
        void run();
		void setMaxClockSpeed(int MaxClockSpeed);
        void setMaxGPUFreq(int MaxGPUFreq);
		void setMqttTopicPrefix(String Prefix);
		void setBackgroundColour(int colour);
		void setDisplayMode(DisplayMode Mode=DarkMode);
		String getMqttTopicPrefix();
        void showCPUPage(ScreenType type=SSD1306);
        void showGPUPage(ScreenType type=SSD1306);
        void showCPUGPUPage(ScreenType type=SSD1306);
		void showMemoryPage(ScreenType type=SSD1306);
		void showVersion(ScreenType type=SSD1306);
		void drawCircleDegree(int x,  int y, int degree = 360, int degreeOld = 0, int color = ST77XX_GREEN, int colorBackground = ST77XX_WHITE, int width = 4, int radius = 20, ScreenType type = ST7735);
		void drawCpuCircle(int x,  int y, int coreIndex, int color = SSD1306_WHITE, ScreenType type=SSD1306);
		void showSixCoreCpu(String cpuName, ScreenType type = ST7735, HardwareType hardware = Intel);
		void printMSG(int lineNumber, const char* text, ScreenType type=SSD1306);
    private:
        WiFiClient esp32;         
        PubSubClient client;     
    	String sBrokerServer;                     
        int iBrokerPort;                          
        int iBrokerQoS = 0;   
		int _iCpuClock[NUM_OF_CORES+1];
		int _iCpuClockOld[NUM_OF_CORES+1];
		int _iCpuLoad[NUM_OF_CORES+1];
		int _iCpuLoadOld[NUM_OF_CORES+1];
		int _iCpuTemp[NUM_OF_CORES+1];
		int _iCpuTempOld[NUM_OF_CORES+1];                   
        String sBrokerClientID;   
        String sBrokerClientName;       
        String sBrokerClientPassword;    
		String _sMqttTopicPrefix = "rpi";
        char cMQTTPayload[100];
		char cScreenBuffer[SCREEN_TEXT_PER_LINE*SCREEN_LINE_OF_TEXT+1];
        String sCpuLoad; 
        String sCpuTemp;
        String sDiskUsage;
        String sMemory;
        String sUptimeHours;
        String sGpuFreq;
        int _iMaxGPUFreq = 600;
        String sSwap;
        String sClockSpeed;
        int _iMaxClockSpeed = 2000;
        Adafruit_SSD1306 *_screen_ssd1306;
		bool _bUseSSD1306 = false;
		Adafruit_ST7735 *_screen_st7735;
		bool _bUseST7735 = false;
		bool connectMQTTBroker(String sClientName, String sClientPassword, String sClientID, String sServer, int iPort);
        void MQTTCallbackFunction(char *topic, byte *payload, unsigned int length); 
		void drawArc(int x, int y, int start_angle, int seg_count, int rx, int ry, int w, int colour, ScreenType type=SSD1306);
		void ssd1306_drawCirclewithIcon(const uint8_t bitmap[],int degree, bool icon_left=true);
		void initScreenBuffer();
		int _iBackgroundColour = ST77XX_MAGENTA;
		DisplayMode _displayMode = DarkMode;

};

const unsigned char bitmapChip64 [] PROGMEM = {
	//64x64px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0d, 0xd9, 0x9b, 0xb0, 0x07, 0xe0, 
	0x00, 0x30, 0x0d, 0xdd, 0xbb, 0xb0, 0x0f, 0xf0, 0x00, 0x30, 0x0d, 0xdd, 0xbb, 0xb0, 0x0e, 0x70, 
	0x00, 0x00, 0x3f, 0xff, 0xff, 0xfc, 0x0c, 0x30, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x0c, 0x30, 
	0x00, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x0f, 0xf0, 0x00, 0x00, 0x70, 0x00, 0x00, 0x0e, 0x07, 0xe0, 
	0x06, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xc0, 0x06, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
	0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 
	0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x1c, 0xff, 0xff, 0xff, 0xff, 0x38, 0x00, 
	0x00, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x00, 0x01, 0xfc, 0xe0, 0x00, 0x00, 0x07, 0x3f, 0x80, 
	0x07, 0xdc, 0xcc, 0x00, 0x00, 0x33, 0x3b, 0xe0, 0x0f, 0xdc, 0xcc, 0x00, 0x00, 0x33, 0x3b, 0xf0, 
	0x0f, 0xdc, 0xcc, 0x00, 0x00, 0x33, 0x3b, 0xf0, 0x01, 0xdc, 0xc0, 0x00, 0x00, 0x03, 0x3b, 0x80, 
	0x0f, 0xdc, 0xc0, 0x00, 0x00, 0x03, 0x3b, 0xf0, 0x0f, 0xdc, 0xc0, 0x7f, 0xfe, 0x03, 0x3b, 0xf0, 
	0x07, 0xdc, 0xc0, 0x7f, 0xfe, 0x03, 0x3b, 0xe0, 0x01, 0xdc, 0xc0, 0x7f, 0xfe, 0x03, 0x3b, 0x80, 
	0x0f, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0xf0, 0x0f, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0xf0, 
	0x01, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0x80, 0x01, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0x80, 
	0x0f, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0xf0, 0x0f, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0xf0, 
	0x01, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0x80, 0x0f, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0xf0, 
	0x0f, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0xf0, 0x0f, 0xdc, 0xc0, 0x60, 0x06, 0x03, 0x3b, 0xf0, 
	0x01, 0xdc, 0xc0, 0x7f, 0xfe, 0x03, 0x3b, 0x80, 0x0f, 0xdc, 0xc0, 0x7f, 0xfe, 0x03, 0x3b, 0xf0, 
	0x0f, 0xdc, 0xc0, 0x00, 0x00, 0x03, 0x3b, 0xf0, 0x01, 0xdc, 0xc0, 0x00, 0x00, 0x03, 0x3b, 0x80, 
	0x01, 0xdc, 0xc0, 0x00, 0x00, 0x03, 0x3b, 0x80, 0x0f, 0xdc, 0xcc, 0x00, 0x00, 0x33, 0x3b, 0xf0, 
	0x0f, 0xdc, 0xcc, 0x00, 0x00, 0x33, 0x3b, 0xf0, 0x01, 0xfc, 0xc0, 0x00, 0x00, 0x03, 0x3f, 0x80, 
	0x01, 0xfc, 0xff, 0xff, 0xff, 0xff, 0x3f, 0x80, 0x00, 0x7c, 0xff, 0xff, 0xff, 0xff, 0x3e, 0x00, 
	0x00, 0x1c, 0x7f, 0xff, 0xff, 0xfe, 0x38, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 
	0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x03, 0x8f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 
	0x07, 0xc7, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x00, 0x0f, 0xe1, 0xff, 0xff, 0xff, 0xff, 0x80, 0x30, 
	0x0c, 0xe0, 0x70, 0x00, 0x00, 0x0e, 0x00, 0x30, 0x0c, 0xe0, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 
	0x0f, 0xe0, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x07, 0xc0, 0x1d, 0xdf, 0xfb, 0xb8, 0x10, 0x00, 
	0x01, 0x00, 0x0d, 0xdd, 0xbb, 0xb0, 0x18, 0x00, 0x00, 0x03, 0x0d, 0xdd, 0xbb, 0xb0, 0x10, 0x00, 
	0x00, 0x03, 0x0c, 0xd9, 0x9b, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapGPU64 [] PROGMEM = {
	//64x64px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x30, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x3f, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xcc, 0x00, 0x01, 0xff, 0xf9, 0xff, 0xe0, 0x00, 0xcc, 0x00, 0x07, 0xff, 0xf9, 0xff, 0xf8, 
	0x00, 0xcc, 0x00, 0x06, 0x00, 0x00, 0x00, 0x18, 0x0f, 0xcc, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 
	0x0f, 0xcf, 0xff, 0xcc, 0xc0, 0x0f, 0xc0, 0xcc, 0x00, 0xcf, 0xff, 0xcc, 0xc0, 0x3f, 0xf0, 0xcc, 
	0x00, 0xcc, 0x00, 0x0c, 0x00, 0xf7, 0x3c, 0x0c, 0x3f, 0xcc, 0x00, 0x0c, 0x01, 0xc6, 0x0e, 0x0c, 
	0x3f, 0xcc, 0x00, 0x0f, 0xc3, 0x86, 0x1f, 0x0c, 0x30, 0xcc, 0xfc, 0x0f, 0xc3, 0xc6, 0x3b, 0x0c, 
	0x30, 0xcc, 0xfc, 0xcc, 0xc6, 0xe6, 0x71, 0x8c, 0x30, 0xcc, 0xcc, 0xcc, 0xc6, 0x77, 0xe1, 0x8c, 
	0x30, 0xcc, 0xcc, 0x0f, 0xcc, 0x3f, 0xc0, 0xcc, 0x30, 0xcc, 0xfc, 0x0f, 0xcc, 0x1f, 0xff, 0xcc, 
	0x30, 0xcc, 0xfc, 0xcc, 0x0e, 0x1c, 0xff, 0xcc, 0x3f, 0xcc, 0x00, 0xcc, 0x0f, 0xfc, 0xe1, 0xcc, 
	0x3f, 0xcc, 0x00, 0xcf, 0xcf, 0xff, 0xe0, 0xcc, 0x00, 0xcc, 0xfc, 0xcf, 0xcc, 0x0f, 0xf0, 0xcc, 
	0x00, 0xcc, 0xfc, 0xcc, 0xc6, 0x1f, 0xb9, 0x8c, 0x0f, 0xcc, 0xcc, 0xcc, 0xc6, 0x39, 0x9d, 0x8c, 
	0x0f, 0xcc, 0xcc, 0xcf, 0xc3, 0x71, 0x8f, 0x0c, 0x00, 0xcc, 0xcc, 0xcf, 0xc3, 0xe1, 0x87, 0x0c, 
	0x00, 0xcc, 0xcc, 0xcc, 0x01, 0xc1, 0x8e, 0x0c, 0x07, 0xcc, 0xcc, 0x0c, 0x00, 0xf3, 0xbc, 0x0c, 
	0x07, 0xcc, 0xcc, 0x0c, 0xc0, 0x3f, 0xf0, 0xcc, 0x00, 0xcc, 0xcc, 0xcc, 0xc0, 0x0f, 0xc0, 0xcc, 
	0x00, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0x00, 0x0c, 0x0f, 0xcc, 0xfc, 0xc6, 0x00, 0x00, 0x00, 0x1c, 
	0x0f, 0xcc, 0xfc, 0xc7, 0xfc, 0xff, 0xff, 0xfc, 0x0c, 0xcc, 0x00, 0x01, 0xfc, 0xff, 0xff, 0xec, 
	0x0c, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x0c, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 
	0x0c, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x0f, 0xcf, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 
	0x0f, 0xcc, 0x33, 0x33, 0x33, 0x33, 0x30, 0x00, 0x00, 0xcc, 0x33, 0x33, 0x33, 0x33, 0x30, 0x00, 
	0x00, 0xcc, 0x3f, 0xff, 0xf3, 0xff, 0xf0, 0x00, 0x07, 0xcc, 0x3f, 0xff, 0xf3, 0xff, 0xf0, 0x00, 
	0x07, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapTiger64 [] PROGMEM = {
	//64x64px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x00, 0x7f, 0xfe, 0x00, 0x0f, 0x00, 
	0x00, 0xff, 0xc1, 0xff, 0xff, 0x83, 0xff, 0x00, 0x01, 0xff, 0xff, 0xfc, 0x3f, 0xff, 0xff, 0x80, 
	0x01, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0x80, 0x03, 0xef, 0xff, 0xc0, 0x03, 0xff, 0xf7, 0xc0, 
	0x03, 0xc7, 0xff, 0x87, 0xe1, 0xff, 0xe3, 0xc0, 0x03, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xc3, 0xc0, 
	0x03, 0xc1, 0xff, 0xff, 0xff, 0xff, 0x83, 0xc0, 0x03, 0xc1, 0xff, 0xff, 0xff, 0xff, 0x83, 0xc0, 
	0x03, 0xe3, 0xff, 0xf8, 0x1f, 0xff, 0xc7, 0x80, 0x01, 0xc7, 0xff, 0xe0, 0x07, 0xff, 0xe3, 0x80, 
	0x01, 0xc7, 0xff, 0x80, 0x01, 0xff, 0xe3, 0x80, 0x00, 0x8f, 0xff, 0x07, 0xe0, 0xff, 0xf1, 0x00, 
	0x00, 0x1f, 0xfe, 0xff, 0xff, 0x7f, 0xf8, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 
	0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 0x00, 
	0x00, 0x7f, 0xff, 0xfc, 0x3f, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xf8, 0x1f, 0xff, 0xfc, 0x00, 
	0x00, 0x3f, 0xc3, 0xf8, 0x1f, 0xc3, 0xfc, 0x00, 0x02, 0x1f, 0xc0, 0xf8, 0x1f, 0x03, 0xf8, 0x40, 
	0x03, 0x9f, 0xe0, 0xf8, 0x1f, 0x07, 0xf9, 0xc0, 0x03, 0xdf, 0xe0, 0x70, 0x0e, 0x07, 0xfb, 0xc0, 
	0x07, 0xff, 0xf8, 0x70, 0x0e, 0x1f, 0xff, 0xe0, 0x07, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xe0, 
	0x03, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0xc0, 0x01, 0xff, 0xff, 0xf0, 0x0f, 0xff, 0xff, 0x80, 
	0x00, 0xff, 0xff, 0xe0, 0x07, 0xff, 0xff, 0x00, 0x00, 0x7f, 0xff, 0xe0, 0x07, 0xff, 0xfe, 0x00, 
	0x08, 0x7f, 0xff, 0xe0, 0x07, 0xff, 0xfe, 0x10, 0x0e, 0x3f, 0xf8, 0x40, 0x02, 0x1f, 0xfc, 0x70, 
	0x0f, 0x3f, 0xe0, 0x1c, 0x38, 0x07, 0xfc, 0xf0, 0x0f, 0xff, 0xc0, 0x1f, 0xf8, 0x03, 0xff, 0xf0, 
	0x07, 0xff, 0xc0, 0x0f, 0xf0, 0x03, 0xff, 0xe0, 0x07, 0xff, 0xc0, 0x07, 0xe0, 0x03, 0xff, 0xe0, 
	0x03, 0xff, 0xc0, 0x03, 0xc0, 0x03, 0xff, 0xc0, 0x01, 0xff, 0xe0, 0x03, 0xc0, 0x07, 0xff, 0x80, 
	0x00, 0x7f, 0xf0, 0x03, 0xc0, 0x0f, 0xfe, 0x00, 0x00, 0x3f, 0xfc, 0x07, 0xe0, 0x3f, 0xfc, 0x00, 
	0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 
	0x00, 0x00, 0xff, 0xc7, 0xe3, 0xff, 0x00, 0x00, 0x00, 0x00, 0x1f, 0x80, 0x01, 0xf8, 0x00, 0x00, 
	0x00, 0x00, 0x07, 0xc0, 0x03, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xe0, 0x07, 0xc0, 0x00, 0x00, 
	0x00, 0x00, 0x03, 0xf8, 0x1f, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0xff, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapRAM32 [] PROGMEM = {
	//32x32px
	0x03, 0xff, 0xff, 0x80, 0x02, 0x10, 0x00, 0x80, 0x03, 0x90, 0x0e, 0x80, 0x00, 0xd0, 0x0a, 0x80, 
	0x00, 0xd0, 0x0e, 0x80, 0x00, 0xd2, 0xa0, 0x80, 0x00, 0xd7, 0xf1, 0x80, 0x00, 0xd4, 0x13, 0x80, 
	0x00, 0xd4, 0x12, 0x80, 0x00, 0xd7, 0xf2, 0x80, 0x00, 0xd2, 0xa2, 0x80, 0x00, 0xd0, 0x02, 0x80, 
	0x00, 0xd0, 0x02, 0x80, 0x00, 0xd2, 0xa2, 0x80, 0x00, 0xd7, 0xf2, 0x80, 0x00, 0xd4, 0x12, 0x80, 
	0x00, 0xd4, 0x12, 0x80, 0x00, 0xd7, 0xf2, 0x80, 0x00, 0xd2, 0xa2, 0x80, 0x00, 0xd0, 0x02, 0x80, 
	0x00, 0xd0, 0x02, 0x80, 0x00, 0xd2, 0xa2, 0x80, 0x00, 0xd7, 0xf2, 0x80, 0x00, 0xd4, 0x12, 0x80, 
	0x00, 0xd4, 0x13, 0x80, 0x00, 0xd7, 0xf1, 0x80, 0x00, 0xd2, 0xa0, 0x80, 0x00, 0xd0, 0x0e, 0x80, 
	0x00, 0xd0, 0x0a, 0x80, 0x03, 0x90, 0x0e, 0x80, 0x02, 0x10, 0x00, 0x80, 0x03, 0xff, 0xff, 0x80
};

const unsigned char bitmapHDD32 [] PROGMEM = {
	//32x32px
	0x0f, 0xff, 0xff, 0xe0, 0x18, 0x00, 0x00, 0x10, 0x10, 0x00, 0x00, 0x08, 0x10, 0x0f, 0xf0, 0x08, 
	0x10, 0x30, 0x0c, 0x08, 0x10, 0x40, 0x06, 0x08, 0x10, 0x80, 0x01, 0x08, 0x11, 0x00, 0x01, 0x88, 
	0x13, 0x00, 0x00, 0x88, 0x12, 0x07, 0xe0, 0x48, 0x12, 0x0c, 0x30, 0x48, 0x16, 0x09, 0x90, 0x48, 
	0x14, 0x12, 0xc8, 0x48, 0x14, 0x12, 0x48, 0x48, 0x14, 0x13, 0xd8, 0x48, 0x12, 0x08, 0x10, 0x48, 
	0x12, 0x0c, 0x20, 0x48, 0x12, 0x03, 0xc0, 0xc8, 0x11, 0x00, 0x00, 0x88, 0x11, 0x80, 0x01, 0x88, 
	0x10, 0xc6, 0x03, 0x08, 0x10, 0x7e, 0x06, 0x08, 0x10, 0x66, 0x18, 0x08, 0x1c, 0x87, 0xe0, 0x08, 
	0x1e, 0x84, 0x00, 0x08, 0x12, 0x88, 0x00, 0x08, 0x13, 0x70, 0x00, 0x08, 0x11, 0x80, 0x00, 0x08, 
	0x10, 0xfc, 0x00, 0x08, 0x10, 0x04, 0x00, 0x08, 0x18, 0x04, 0x00, 0x10, 0x0f, 0xff, 0xff, 0xe0
};

const unsigned char bitmapSwap32[] PROGMEM = {
	//32x32px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0x60, 0x00, 0x00, 0x38, 0x08, 0x00, 
	0x00, 0xc0, 0x02, 0x00, 0x01, 0x80, 0x00, 0x00, 0x02, 0x00, 0x00, 0x70, 0x04, 0x00, 0x00, 0x7e, 
	0x0c, 0x00, 0x00, 0x70, 0x08, 0x00, 0x00, 0x50, 0x10, 0x00, 0x00, 0x48, 0x10, 0x00, 0x00, 0xc8, 
	0x30, 0x00, 0x00, 0x0c, 0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 
	0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x20, 0x00, 0x00, 0x04, 0x30, 0x00, 0x00, 0x0c, 
	0x13, 0x00, 0x00, 0x08, 0x12, 0x00, 0x00, 0x08, 0x0a, 0x00, 0x00, 0x10, 0x0e, 0x00, 0x00, 0x30, 
	0x7e, 0x00, 0x00, 0x20, 0x0e, 0x00, 0x00, 0x40, 0x00, 0x00, 0x01, 0x80, 0x00, 0x40, 0x03, 0x00, 
	0x00, 0x10, 0x1c, 0x00, 0x00, 0x06, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapIntel48[] PROGMEM = {
	//48x48px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x7f, 0xf8, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x00, 0x7e, 0x00, 
	0x07, 0xe0, 0x00, 0x01, 0xc0, 0x00, 0x00, 0xf0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x38, 0x00, 0x18, 
	0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0x00, 0x00, 0x00, 0x00, 0x06, 0x06, 
	0x01, 0x80, 0x00, 0x00, 0x06, 0x07, 0x01, 0x80, 0x03, 0x00, 0x06, 0x07, 0x08, 0x00, 0x03, 0x00, 
	0x06, 0x03, 0x10, 0x00, 0x03, 0x03, 0x06, 0x03, 0x31, 0x8f, 0xe3, 0xcf, 0xc6, 0x07, 0x21, 0x8f, 
	0xe3, 0x9f, 0xc6, 0x07, 0x61, 0x8c, 0x73, 0x18, 0xe6, 0x06, 0x41, 0x8c, 0x73, 0x18, 0xe6, 0x0e, 
	0xc1, 0x8c, 0x73, 0x3f, 0xe6, 0x1c, 0xc1, 0x8c, 0x73, 0x18, 0x06, 0x3c, 0xc1, 0x8c, 0x73, 0x18, 
	0x06, 0x78, 0xc1, 0x8c, 0x73, 0xdf, 0xc6, 0x70, 0xc0, 0x8c, 0x73, 0xcf, 0xc6, 0x60, 0xc0, 0x00, 
	0x00, 0x01, 0x00, 0x00, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x70, 0x00, 0x00, 0x00, 0x03, 0x00, 0x38, 0x00, 0x00, 0x00, 0x1f, 0x00, 0x3e, 0x00, 0x00, 0x00, 
	0xff, 0x00, 0x1f, 0x80, 0x00, 0x07, 0xfe, 0x00, 0x07, 0xfc, 0x01, 0xff, 0xf8, 0x00, 0x01, 0xff, 
	0xff, 0xff, 0xc0, 0x00, 0x00, 0x7f, 0xff, 0xfc, 0x00, 0x00, 0x00, 0x07, 0xff, 0x80, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapAMD50[] PROGMEM = {
	//50x50px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xff, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0xff, 0xff, 0xfc, 
	0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 
	0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x3f, 0xff, 
	0xff, 0xff, 0xfc, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 
	0xfc, 0x00, 0x00, 0x07, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xfc, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x03, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x07, 0x00, 0x00, 
	0x3f, 0xfc, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x3f, 0xfc, 
	0x00, 0x00, 0x3f, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x00, 
	0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x01, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x03, 0xff, 0x00, 
	0x00, 0x3f, 0xfc, 0x00, 0x07, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x3f, 
	0xfc, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 
	0x0f, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x3f, 0xfc, 0x00, 0x0f, 0xff, 
	0xff, 0xfe, 0x3f, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xfe, 0x3f, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xfc, 
	0x1f, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xf8, 0x0f, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0x07, 0xfc, 
	0x00, 0x0f, 0xff, 0xff, 0xe0, 0x03, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0xc0, 0x01, 0xfc, 0x00, 0x0f, 
	0xff, 0xff, 0x80, 0x00, 0xfc, 0x00, 0x0f, 0xff, 0xff, 0x00, 0x00, 0x7c, 0x00, 0x0f, 0xff, 0xfe, 
	0x00, 0x00, 0x3c, 0x00, 0x0f, 0xff, 0xfc, 0x00, 0x00, 0x1c, 0x00, 0x0f, 0xff, 0xf8, 0x00, 0x00, 
	0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char bitmapRyzen24[] PROGMEM = {
	// 24x24px
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf4, 0xbd, 
	0xe9, 0x13, 0x08, 0x0d, 0xa2, 0x11, 0x0b, 0x92, 0x3d, 0xe9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#endif	// _ESPJARVIS_H