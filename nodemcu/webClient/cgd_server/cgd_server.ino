/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : cgd_server
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200228
程序目的/Purpose          : 
演示如何实现两个NodeMCU间通过WiFi进行通讯。两台NodeMCU一个以服务器模式运行，
一个以客户端模式运行。用户可通过服务器端开发板上按钮控制客户端的板上LED点亮和熄灭

此代码为服务器端代码。此代码主要功能：
  - 实时获取服务器端开发板上按钮引脚状态
  - 当有客户端向ESP8266服务器的/update发送请求时，将服务器的按键引脚状态
    通过响应信息回复给客户端
***********************************************************************/
/*
1.没有第二块板子可以使用浏览器来发送请求，查看响应：
浏览器输入：192.168.0.123/update
默认情况返回（输入上拉模式）：buttonState: 1
按下flash按键返回：buttonState: 0
*/

#include <ESP8266WiFi.h>        // 本程序使用 ESP8266WiFi库
#include <ESP8266WiFiMulti.h>   //  ESP8266WiFiMulti库
#include <ESP8266WebServer.h>   //  ESP8266WebServer库

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27

#define buttonPin D3            // 按钮引脚D3

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'
 
ESP8266WebServer esp8266_server(80);// 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）

IPAddress local_IP(192, 168, 31, 123); // 设置ESP8266-NodeMCU联网后的IP
IPAddress gateway(192, 168, 31, 1);    // 设置网关IP（通常网关IP是WiFI路由IP）
IPAddress subnet(255, 255, 255, 0);   // 设置子网掩码
IPAddress dns(192,168,31,1);           // 设置局域网DNS的IP（通常局域网DNS的IP是WiFI路由IP）
 
bool pinState;                 // 存储引脚状态用变量

void setup(){

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(100);

  //mySerial.begin(9600);          // 启动串口通讯
  mySerial.println("");

  pinMode(buttonPin, INPUT_PULLUP); // 将按键引脚设置为输入上拉模式

  // 设置开发板网络环境
  if (!WiFi.config(local_IP, gateway, subnet)) {
    mySerial.println("Failed to ESP8266 IP"); 
  } 

  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP("Xiaomi_FD94", "LuckDuck2^8");
  wifiMulti.addAP("TP-LINK_68EFEC", "18163676911*");
  wifiMulti.addAP("taichi-maker1", "12345678"); // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
  wifiMulti.addAP("taichi-maker2", "87654321"); // 用户可自行增加、修改、删除此处网络连接信息。
  wifiMulti.addAP("taichi-maker3", "13572468"); 

  // 尝试网络连接                                            
  int i = 0;                                 
  while (wifiMulti.run() != WL_CONNECTED) {  
    delay(1000);                             
    mySerial.print(i++); mySerial.print(' ');   
  }                                          
                                             
  // WiFi连接成功后，通过串口监视器输出连接信息 
  mySerial.println("");                     
  mySerial.print("Connected to ");            
  mySerial.println(WiFi.SSID());              
  mySerial.print("IP address:\t");            
  mySerial.println(WiFi.localIP());           

  // 设置网络服务器                  
  esp8266_server.on("/update", handleUpdate);  // 处理客户端HTTP请求LED状态  
  
  esp8266_server.begin(); 
  mySerial.println("HTTP esp8266_server started");
}

void loop(){
  pinState = digitalRead(buttonPin); // 获取引脚状态
  esp8266_server.handleClient();     // 处理http服务器访问 
}          

// 处理客户端HTTP请求LED状态button: 1
void handleUpdate() {
  String pinStateStr;
  pinState == HIGH ? pinStateStr = "1" : pinStateStr = "0";
  esp8266_server.send(200, "text/html", "buttonState: " + pinStateStr); 
}
