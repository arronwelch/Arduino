/**********************************************************************
  项目名称/Project          : 零基础入门学用物联网
  程序名称/Program name     : HTTPClient_request_basic
  团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
  作者/Author              : Dapenson
  日期/Date（YYYYMMDD）     : 20200325
  程序目的/Purpose          :
  此程序用于演示如何使用ESP8266来向www.example.com网站服务器发送HTTP请求并通过串口
  监视器将网站服务器响应信息通过串口监视器显示出来。
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
#include <ESP8266HTTPClient.h>

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27
 
// 测试HTTP请求用的URL。注意网址前面必须添加"http://"
#define URL "http://www.example.com"
 
// 设置wifi接入信息(请根据您的WiFi信息进行修改)
const char* ssid = "testwifi";
const char* password = "123456@**";
 
void setup() {
  //初始化串口设置
  //Serial.begin(9600);

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
 
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
  mySerial.print("WiFi Connected!");
  
  httpClientRequest();  
}
 
void loop() {}
 
// 发送HTTP请求并且将服务器响应通过串口输出
void httpClientRequest(){

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
 
  //重点1 创建 HTTPClient 对象
  HTTPClient httpClient;

  //重点2 通过begin函数配置请求地址。此处也可以不使用端口号和PATH而单纯的
  httpClient.begin(URL); 
  mySerial.print("URL: "); mySerial.println(URL);
 
  //重点3 通过GET函数启动连接并发送HTTP请求
  int httpCode = httpClient.GET();
  mySerial.print("Send GET request to URL: ");
  mySerial.println(URL);
  
  //重点4. 如果服务器响应HTTP_CODE_OK(200)则从服务器获取响应体信息并通过串口输出
  //如果服务器不响应HTTP_CODE_OK(200)则将服务器响应状态码通过串口输出
  if (httpCode == HTTP_CODE_OK) {
    // 使用getString函数获取服务器响应体内容
    String responsePayload = httpClient.getString();
    mySerial.println("Server Response Payload: ");
    mySerial.println(responsePayload);
  } else {
    mySerial.println("Server Respose Code：");
    mySerial.println(httpCode);
  }
 
  //重点5. 关闭ESP8266与服务器连接
  httpClient.end();
}
