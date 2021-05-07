/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : wifiClient_request_basic
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : Dapenson
日期/Date（YYYYMMDD）     : 20200325
程序目的/Purpose          : 
此程序用于演示如何使用esp8266作为互联网客户端向www.example.com网站服务器发送http请求，
并且将网站服务器响应的信息输出在屏幕中
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200411      CYNO朔           001        将请求服务器改为www.example.com
-----------------------------------------------------------------------
本示例程序为太极创客团队制作的《零基础入门学用物联网》中示例程序。
该教程为对物联网开发感兴趣的朋友所设计和制作。如需了解更多该教程的信息，请参考以下网页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-nodemcu-web-client/http-request/
***********************************************************************/
#include <ESP8266WiFi.h>

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27

const char* host = "www.example.com"; // 网络服务器地址
const int httpPort = 80;              // http端口80

// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "TP-LINK_68EFEC";
const char* password = "18163676911*";

void setup() {

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  //初始化串口设置
  //mySerial.begin(9600);
  mySerial.println("");
 
  //设置ESP8266工作模式为无线终端模式
  WiFi.mode(WIFI_STA);
 
  //开始连接wifi
  WiFi.begin(ssid, password);
 
  //等待WiFi连接,连接成功打印IP
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    mySerial.print(".");
  }
  mySerial.println("");
  mySerial.println("WiFi Connected!");
  
  wifiClientRequest();  
}

void loop(){}

// 向服务器发送HTTP请求
void wifiClientRequest(){

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  
  // 建立WiFi客户端对象，对象名称client
  WiFiClient client;    

  // 建立字符串，用于HTTP请求
  String httpRequest =  String("GET /") + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n" +
                        "\r\n";
  
  // 通过串口输出连接服务器名称以便查阅连接服务器的网址                      
  mySerial.print("Connecting to "); 
  mySerial.print(host); 

  // 连接网络服务器，以下段落中的示例程序为本程序重点1
  // 请参考太极创客网站中关于本程序的讲解页面获取详细说明信息。网址：
  // http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-nodemcu-web-client/http-request/
  if (client.connect(host, httpPort)){ 
    mySerial.println(" Success!");        // 连接成功后串口输出“Success”信息
    
    client.print(httpRequest);          // 向服务器发送HTTP请求
    mySerial.println("Sending request: ");// 通过串口输出HTTP请求信息内容以便查阅
    mySerial.println(httpRequest);     
    
    // 通过串口输出网络服务器响应信息， 以下段落中的示例程序为本程序重点2
    // 请参考太极创客网站中关于本程序的讲解页面获取详细说明信息。网址：
    // http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-nodemcu-web-client/http-request/
    mySerial.println("Web Server Response:");        
    while (client.connected() || client.available()){ 
      if (client.available()){
        String line = client.readStringUntil('\n');
        mySerial.println(line);
      }
    }
    
    client.stop();                      // 断开与服务器的连接
    mySerial.print("Disconnected from "); // 并且通过串口输出断开连接信息
    mySerial.print(host);
    
  } else{    // 如果连接不成功则通过串口输出“连接失败”信息
    mySerial.println(" connection failed!");
    client.stop();
  }  
}
