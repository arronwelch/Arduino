/**********************************************************************
项目名称/Project           : 零基础入门学用物联网
程序名称/Program name      : esp8266-flash-append
团队/Team                 : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author               : CYNO 朔 
日期/Date（YYYYMMDD）      : 20191109
程序目的/Purpose           : 此程序用于演示如何向NodeMCU的内置SPIFFS中存储的文件
                            notes.txt添加数据。                      
-----------------------------------------------------------------------  
函数说明：
SPIFFS.open(file_name, "a"); 
以上SPIFFS函数有两个参数：
第一个参数是被操作的文件名称，本示例中该文件为/notes.txt
第二个参数"a" 代表添加文件信息。（如需了解如何读取信息，请参阅示例程序esp8266-flash-read）
此示例程序所演示的是向SPIFFS中的文件里添加信息。这一操作写入信息有所区别。
添加信息是不会删除文件内原有信息，而是在原有信息后面添加新的信息。
但写入操作（示例 esp8266-flash-write.ino）是将文件内容完全清除，重新写入新信息。    
***********************************************************************/
 
#include <FS.h>
#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
String htmlCode = "";                   // 存储文件内容的字符串变量

String file_name = "/taichi-maker/notes.txt";              //被读取的文件位置和名称
 
void setup() {
  // Serial.begin(9600);
  // Serial.println("");

  //通过addAp函数存储  WiFi名称       WiFi密码
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

  if(SPIFFS.begin()){ // 启动闪存文件系统
    //Serial.println("SPIFFS Started.");
    htmlCode += "2.SPIFFS Started||";
  } else {
    //Serial.println("SPIFFS Failed to Start.");
    htmlCode += "2.SPIFFS Failed to Start||";
  }
 
  //确认闪存中是否有file_name文件
  if (SPIFFS.exists(file_name)){
    
    //Serial.print(file_name);
    //Serial.println(" FOUND.");
    htmlCode += "3.FOUND ";
    htmlCode += file_name;
    htmlCode += "||";

    File dataFile = SPIFFS.open(file_name, "a");// 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
    dataFile.println("This is Appended Info."); // 向dataFile添加字符串信息
    dataFile.close();                           // 完成文件操作后关闭文件   
    //Serial.println("Finished Appending data to SPIFFS");
    htmlCode +="4.Finished Appending data to SPIFFS||";
    
  } else {
    //Serial.print(file_name);
    //Serial.print(" NOT FOUND.");
    htmlCode += "3.NOT FOUND ";
    htmlCode += file_name;
    htmlCode += "||";
    htmlCode +="4.NOT Finished Appending data to SPIFFS||";
  }
                        
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