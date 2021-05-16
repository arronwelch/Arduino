/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : OTA_Flash_Example
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200713
程序目的/Purpose          : 
本示例程序用于演示ESP8266的OTA功能。通过本程序，我们将启动ESP8266的OTA程序上传功能。
我们将无需使用连接线将ESP8266与电脑进行连接，而可以使用WiFi对ESP8266上传程序。

如需了解本程序的详细说明，请参考以下链接：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-tips/esp8266-ota/
-----------------------------------------------------------------------
其它说明 / Other Description：
本程序为太极创客团队制作的免费视频教程《零基础入门学用物联网 》中一部分。该教程系统的
向您讲述ESP8266的物联网应用相关的软件和硬件知识。以下是该教程目录页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/                    
***********************************************************************/
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>
#include <Ticker.h>

// 闪烁时间间隔(秒)
const int blinkInterval = 3; 

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "Xiaomi_FD94";
const char* password = "LuckDuck2^8";

Ticker ticker;

void setup() {
  Serial.begin(9600);            
  Serial.println("");
  pinMode(LED_BUILTIN, OUTPUT);

  ticker.attach(blinkInterval, tickerCount);  // 设置Ticker对象
  
  connectWifi();

  // OTA设置并启动
  ArduinoOTA.setHostname("ESP8266");
  ArduinoOTA.setPassword("12345678");
  ArduinoOTA.begin();
  
  Serial.println("OTA ready");
}
void loop() {
  ArduinoOTA.handle();
}

// 在Tinker对象控制下，此函数将会定时执行。
void tickerCount(){
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void connectWifi(){
  //开始连接wifi
  WiFi.begin(ssid, password);
 
  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected!");  
  Serial.print("IP address:\t");            
  Serial.println(WiFi.localIP());          
}
