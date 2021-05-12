/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : cgj_server_1
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 2020305
程序目的/Purpose          : 
本实例用于演示esp8266的json数据通讯。
操作测试本程序需要使用两台8266开发板。其中一台为服务器端，一台为客户端。
本程序为服务器程序，功能如下：

1. 实时读取A0、 D1、D2以及D3引脚的读数。
2. 当有客户端请求信息时，将会通过http响应将引脚读数等信息发送给客户端。
   信息发送格式为json格式。
3. 本程序使用了wifi.config对开发板的IP进行了配置。
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200511      CYNO朔           001        1 移除handleNotFound使教程代码更加精简
                                          2 改请求路径为update
***********************************************************************/

/*
    {
        "info": {
            "name": "taichimaker",
            "url": "www.taichi-maker.com",
            "email": "taichimaker@163.com"
        },
        "digital_pin": {
            "d1": "1",
            "d2": "0",
            "d3": "1"
        },
        "analog_pin": {
            "a0": "500"
        }
    }
 */

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27

#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库
#include <ArduinoJson.h>

#define buttonPin D3            // 按钮引脚D3

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
 
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

IPAddress local_IP(192, 168, 31, 150); // 设置ESP8266-NodeMCU联网后的IP
IPAddress gateway(192, 168, 31, 1);    // 设置网关IP（通常网关IP是WiFI路由IP）
IPAddress subnet(255, 255, 255, 0);   // 设置子网掩码
IPAddress dns(192,168,31,1);           // 设置局域网DNS的IP（通常局域网DNS的IP是WiFI路由IP）
         
void setup(){
  Serial.begin(9600);          // 启动串口通讯
  Serial.println("");

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(100);       
  mySerial.println("");

  // 将引脚设置为输入上拉模式
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(buttonPin, INPUT_PULLUP);   // NodeMCU开发板按键连接在D3引脚上（flash按键）
   
  // 设置开发板网络环境
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to Config ESP8266 IP"); 
    mySerial.println("Failed to Config ESP8266 IP"); 
  } 

  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP("Xiaomi_FD94", "LuckDuck2^8"); // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
  wifiMulti.addAP("taichi-maker1", "12345678"); // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
  wifiMulti.addAP("taichi-maker2", "87654321"); // 这3个WiFi网络名称分别是taichi-maker, taichi-maker2, taichi-maker3。
  wifiMulti.addAP("taichi-maker3", "13572468"); // 这3个网络的密码分别是123456789，87654321，13572468。
                                                // 此处WiFi信息只是示例，请在使用时将需要连接的WiFi信息填入相应位置。
                                                // 另外这里只存储了3个WiFi信息，您可以存储更多的WiFi信息在此处。

  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  // 此处的wifiMulti.run()是重点。通过wifiMulti.run()，NodeMCU将会在当前
    delay(1000);                             // 环境中搜索addAP函数所存储的WiFi。如果搜到多个存储的WiFi那么NodeMCU
    Serial.print(i++); Serial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
    mySerial.print(i++); mySerial.print(' ');    // 将会连接信号最强的那一个WiFi信号。
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址

  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  mySerial.println('\n');                     // WiFi连接成功后
  mySerial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  mySerial.println(WiFi.SSID());              // 连接的WiFI名称
  mySerial.print("IP address:\t");            // 以及
  mySerial.println(WiFi.localIP());           // NodeMCU的IP地址

  esp8266_server.on("/", handleRoot);          
  esp8266_server.begin();  
  
  Serial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
  mySerial.println("HTTP esp8266_server started");//  告知用户ESP8266网络服务功能已经启动
}

void loop(){
  // 处理http服务器访问
  esp8266_server.handleClient(); 
}                                                                   

void handleRoot() {   //处理网站目录“/”的访问请求 
  esp8266_server.send(200, "application/json", rootJson());  
}

// // 实时获取ESP8266开发板引脚信息并且建立JSON信息
// // 以便ESP8266服务器通过响应信息发送给客户端
// String rootJson(){

//   // define pin modes for tx, rx:
//   pinMode(rxPin, INPUT);
//   pinMode(txPin, OUTPUT);
//   // set up a new serial port
//   SoftwareSerial mySerial(rxPin, txPin);
//   // set the data rate for the SoftwareSerial port
//   mySerial.begin(9600);
//   delay(100);  

//   String jsonCode = "{\"info\": {\"name\": \"taichimaker\",\"url\": \"www.taichi-maker.com\",\"email\": \"taichimaker@163.com\"},\"digital_pin\": {\"d1\": \"";
//   jsonCode += String(digitalRead(D1));  
//   jsonCode += "\",\"d2\": \""; 
//   jsonCode += String(digitalRead(D2));  
//   jsonCode += "\",\"d3\": \""; 
//   jsonCode += String(digitalRead(D3));  
//   jsonCode += "\"},\"analog_pin\": {\"a0\": \"";
//   jsonCode += String(analogRead(A0));
//   jsonCode += "\"}}";  
  
//   Serial.print("jsonCode: ");Serial.println(jsonCode);
//   mySerial.print("jsonCode: ");mySerial.println(jsonCode);
  
//   return jsonCode;
// }







// 实时获取ESP8266开发板引脚信息并且建立JSON信息
// 以便ESP8266服务器通过响应信息发送给客户端
// https://arduinojson.org/v6/assistant/
/*
  Step 1: Configuration
  Processor:    ESP8266
  Mode:         Serialize
  Output type:  String
  This is the Assistant for ArduinoJson 6.18.0. 
  Make sure the same version is installed on your computer.
 */
String rootJson(){

  StaticJsonDocument<384> doc;

  JsonObject info = doc.createNestedObject("info");
  info["name"] = "taichimaker";
  info["url"] = "www.taichi-maker.com";
  info["email"] = "taichimaker@163.com";

  JsonObject digital_pin = doc.createNestedObject("digital_pin");
  digital_pin["d1"] = String(digitalRead(D1));
  digital_pin["d2"] = String(digitalRead(D2));
  digital_pin["d3"] = String(digitalRead(D3));
  doc["analog_pin"]["a0"] = String(analogRead(A0));

  String jsonCode;
  serializeJson(doc, jsonCode);

  return jsonCode;
}