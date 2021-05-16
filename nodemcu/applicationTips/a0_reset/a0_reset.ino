/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : a0_reset
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200703
程序目的/Purpose          : 
清除ESP8266的闪存中所存储的WiFi连接信息。如需获取本示例程序的详细说明，请前往以下链接：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-tips/wifimanager/
-----------------------------------------------------------------------
其它说明 / Other Description：
本程序为太极创客团队制作的免费视频教程《零基础入门学用物联网 》中一部分。该教程系统的
向您讲述ESP8266的物联网应用相关的软件和硬件知识。以下是该教程目录页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/                    
***********************************************************************/
#include <ESP8266WiFi.h>          
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         

void setup() {
  Serial.begin(9600);       
  // 建立WiFiManager对象
  WiFiManager wifiManager;
  
  // 清除ESP8266所存储的WiFi连接信息以便测试WiFiManager工作效果
  wifiManager.resetSettings();
  Serial.println("ESP8266 WiFi Settings Cleared");
}

void loop() {}