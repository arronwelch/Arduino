/**********************************************************************
项目名称/Project           : 零基础入门学用物联网
程序名称/Program name      : esp8266-flash-write
团队/Team                 : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author               : CYNO 朔 
日期/Date（YYYYMMDD）      : 20191109
程序目的/Purpose           : 此程序用于演示如何向NodeMCU的SPIFFS中建立名为
                            notes.txt的文件，程序还将向该文件写入信息。
-----------------------------------------------------------------------
函数说明：
SPIFFS.open(file_name, "w"); 
以上函数有两个参数：
第一个参数是被操作的文件名称，本示例中该文件为/notes.txt
第二个参数"w" 代表写入文件信息。（如需了解如何读取信息，请参阅示例程序esp8266-flash-read）
***********************************************************************/
 
 
#include <FS.h>  
#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

String file_name = "/taichi-maker/notes.txt"; //被读取的文件位置和名称
String htmlCode = "";                   // 存储文件内容的字符串变量

void setup() {

  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP("TP-LINK_68EFEC","18163676911*");
//wifiMulti.addAP("TP-LINK_DB58","nch13787252353");
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
//Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。
  htmlCode +="||1.Connecting wifi"; 

  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
  //Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
    htmlCode += "."; 
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。                                          
  htmlCode +="||";

  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  // Serial.println('\n');                     // WiFi连接成功后
  // Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  // Serial.println(WiFi.SSID());              // 连接的WiFI名称
  // Serial.print("IP address:\t");            // 以及
  // Serial.println(WiFi.localIP());           // NodeMCU的IP地址

  esp8266_server.begin();                   // 启动网站服务                
  esp8266_server.on("/", handleRoot);       // 设置服务器根目录即'/'的函数'handleRoot'
  esp8266_server.onNotFound(handleNotFound);// 设置处理404情况的函数'handleNotFound'  

  //Serialprintln("SPIFFS format start");  
  SPIFFS.format();    // 格式化SPIFFS
  //Serialprintln("SPIFFS format finish");
  htmlCode +="2.SPIFFS format finish||"; 

  if(SPIFFS.begin()){ // 启动SPIFFS
    //Serialprintln("SPIFFS Started.");
    htmlCode +="3.SPIFFS Started||";
  } else {
    //Serialprintln("SPIFFS Failed to Start.");
    htmlCode +="3.SPIFFS Failed to Start||";
  }
  
  File dataFile = SPIFFS.open(file_name, "w");// 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
  dataFile.println("Hello IOT World.Test ID:3333");       // 向dataFile写入字符串信息
  dataFile.close();                           // 完成文件写入后关闭文件
  //Serialprintln("Finished Writing data to SPIFFS");
  htmlCode +="4.Finished Writing data to SPIFFS||";
}
 
void loop() {
  esp8266_server.handleClient();     // 处理http服务器访问
}

void handleRoot() {   
  esp8266_server.send(200, "text/plain", htmlCode); 

                                            // 向浏览器发送按键状态信息  
}
 
// 设置处理404情况的函数'handleNotFound'
void handleNotFound(){                                        // 当浏览器请求的网络资源无法在服务器找到时，
  esp8266_server.send(404, "text/plain", "404: Not found");   // NodeMCU将调用此函数。
}
