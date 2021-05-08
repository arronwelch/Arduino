/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : csd_client
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200228
程序目的/Purpose          : 
演示如何实现两个NodeMCU间通过WiFi进行通讯。两台NodeMCU一个以服务器模式运行，
一个以客户端模式运行。

此代码为客户端代码。此代码主要功能：
    - 通过HTTP协议向服务器发送HTTP请求
    - HTTP请求中包含客户端按键开关引脚状态和程序测试数据
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
-----------------------------------------------------------------------
http信息发送说明
192.168.0.123/update?float=1.5&int=2&button=0
***********************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h> // 使用WiFiMulti库

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27

#define buttonPin D3 // 按钮引脚D3

ESP8266WiFiMulti wifiMulti; // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'

bool buttonState;       //存储客户端按键控制数据
float clientFloatValue; //存储客户端发送的浮点型测试数据
int clientIntValue;     //存储客户端发送的整数型测试数据

const char *host = "192.168.0.123"; // 即将连接服务器网址/IP
const int httpPort = 80;            // 即将连接服务器端口

void setup(void)
{
  // Serial.begin(9600);                  // 启动串口通讯
  // Serial.println("");

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("");

  pinMode(buttonPin, INPUT_PULLUP); // 将按键引脚设置为输入上拉模式

  wifiMulti.addAP("TP-LINK_68EFEC", "18163676911*");
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  mySerial.println("Connecting ...");                          // 则尝试使用此处存储的密码进行连接。

  while (wifiMulti.run() != WL_CONNECTED)
  { // 尝试进行wifi连接。
    delay(250);
    mySerial.print('.');
  }

  // WiFi连接成功后将通过串口监视器输出连接成功信息
  mySerial.println('\n');
  mySerial.print("Connected to ");
  mySerial.println(WiFi.SSID()); // 通过串口监视器输出连接的WiFi名称
  mySerial.print("IP address:\t");
  mySerial.println(WiFi.localIP()); // 通过串口监视器输出ESP8266-NodeMCU的IP
}

void loop(void)
{
  // 获取按键引脚状态
  buttonState = digitalRead(buttonPin);

  // 改变测试用变量数值用于服务器端接收数据检测
  clientFloatValue += 1.5;
  clientIntValue += 2;

  // 发送请求
  wifiClientRequest();
  delay(1000);
}

void wifiClientRequest()
{

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  WiFiClient client;

  // 将需要发送的数据信息放入客户端请求
  String url = "/update?float=" + String(clientFloatValue) +
               "&int=" + String(clientIntValue) +
               "&button=" + String(buttonState);

  // 建立字符串，用于HTTP请求
  String httpRequest = String("GET ") + url + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n" +
                       "\r\n";

  mySerial.print("Connecting to ");
  mySerial.print(host);

  if (client.connect(host, httpPort))
  { //如果连接失败则串口输出信息告知用户然后返回loop
    mySerial.println(" Sucess");

    client.print(httpRequest);             // 向服务器发送HTTP请求
    mySerial.println("Sending request: "); // 通过串口输出HTTP请求信息内容以便查阅
    mySerial.println(httpRequest);
  }
  else
  {
    mySerial.println(" failed");
  }

  client.stop();
}