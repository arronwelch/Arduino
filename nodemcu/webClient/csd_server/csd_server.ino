/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : csd_server
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200228
程序目的/Purpose          : 
演示如何实现两个NodeMCU间通过WiFi进行通讯。两台NodeMCU一个以服务器模式运行，
一个以客户端模式运行。
 
此代码为服务器代码。此代码主要功能：
    - 接收客户端发来的http请求
    - 解析客户端请求中的数据信息
    - 将解析的数据信息通过串口监视器显示供用户查看
    - 将解析的客户端按键状态信息用于控制板上LED的点亮和熄灭
    
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200229      CYNO朔           0.01       加入IP设置
 
***********************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> // 使用WiFiMulti库
#include <ESP8266WebServer.h> // 使用WebServer库

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27

ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'

ESP8266WebServer server(80); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

IPAddress local_IP(192, 168, 0, 123); // 设置ESP8266-NodeMCU联网后的IP
IPAddress gateway(192, 168, 0, 1);    // 设置网关IP（通常网关IP是WiFI路由IP）
IPAddress subnet(255, 255, 255, 0);   // 设置子网掩码
IPAddress dns(192, 168, 0, 1);        // 设置局域网DNS的IP（通常局域网DNS的IP是WiFI路由IP）

void setup(void)
{
  //   Serial.begin(9600);          // 启动串口通讯
  //   Serial.println("");

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  // 设置开发板网络环境
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    mySerial.println("Failed to Config ESP8266 IP");
  }
  wifiMulti.addAP("TP-LINK_68EFEC", "18163676911*");
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1");
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  mySerial.println("Connecting ...");                          // 则尝试使用此处存储的密码进行连接。

  // 尝试进行wifi连接。
  while (wifiMulti.run() != WL_CONNECTED)
  {
    delay(250);
    mySerial.print('.');
  }

  // WiFi连接成功后将通过串口监视器输出连接成功信息
  mySerial.println('\n');
  mySerial.print("Connected to ");
  mySerial.println(WiFi.SSID()); // 通过串口监视器输出连接的WiFi名称
  mySerial.print("IP address:\t");
  mySerial.println(WiFi.localIP()); // 通过串口监视器输出ESP8266-NodeMCU的IP

  server.on("/update", handleUpdate); // 处理服务器更新函数

  server.begin(); // 启动网站服务
  mySerial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient(); // 检查http服务器访问
}

void handleUpdate()
{

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  float floatValue = server.arg("float").toFloat(); // 获取客户端发送HTTP信息中的浮点数值
  int intValue = server.arg("int").toInt();         // 获取客户端发送HTTP信息中的整数数值
  int buttonValue = server.arg("button").toInt();   // 获取客户端发送HTTP信息中的按键控制量

  server.send(200, "text/plain", "Received"); // 发送http响应

  buttonValue == 0 ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);

  // 通过串口监视器输出获取到的变量数值
  mySerial.print("floatValue = ");
  mySerial.println(floatValue);
  mySerial.print("intValue = ");
  mySerial.println(intValue);
  mySerial.print("buttonValue = ");
  mySerial.println(buttonValue);
  mySerial.println("=================");
}