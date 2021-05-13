/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : cgj_client_2
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200228
程序目的/Purpose          : 
本实例用于演示esp8266的json数据通讯。
操作测试本程序需要使用两台8266开发板。其中一台为服务器端，一台为客户端。
本程序为客户端程序，功能如下：

1. 通过http协议向服务器端请求json数据信息
2. 解析服务器端响应的json信息内容。
3. 将解析后的数据信息显示于串口监视器
4. 利用服务器端D3引脚（按键引脚）读数来控制客户端开发板上LED的点亮和熄灭
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200302      CYNO朔           001        添加arduinojson解析错误识别
20200511      CYNO朔           002        改请求路径为update
20200511      CYNO朔           003        parse过程使用函数完成
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

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;           // 建立ESP8266WiFiMulti对象

const char* host = "192.168.31.150";   // 将要连接的服务器地址  
const int httpPort = 80;              // 将要连接的服务器端口      

void setup(){
  Serial.begin(9600);          
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
  
  // 设置开发板LED引脚
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  wifiMulti.addAP("Xiaomi_FD94", "LuckDuck2^8"); // 这三条语句通过调用函数addAP来记录3个不同的WiFi网络信息。
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
  Serial.println("Connecting ..."); 
  mySerial.println("Connecting ..."); 

  int i = 0;  
  while (wifiMulti.run() != WL_CONNECTED) { // 尝试进行wifi连接。
    delay(1000);
    Serial.print(i++); Serial.print(' ');
    mySerial.print(i++); mySerial.print(' ');
  }
  
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // IP

  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  mySerial.println("");
  mySerial.print("Connected to ");
  mySerial.println(WiFi.SSID());              // WiFi名称
  mySerial.print("IP address:\t");
  mySerial.println(WiFi.localIP());           // IP
}

void loop(){
  httpRequest("/");
  delay(2000);
  httpRequest("/info");
  delay(2000);
  httpRequest("/digital_pin");
  delay(2000);
}

// 向服务器请求信息并对信息进行解析
void httpRequest(String url){

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  WiFiClient client;

  String httpRequest = String("GET ") + url + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
   
  Serial.print("Connecting to "); Serial.print(host);
  mySerial.print("Connecting to "); mySerial.print(host);
  
  if (client.connect(host, 80)){
    Serial.println(" Success!");
    mySerial.println(" Success!");

    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest); 
    mySerial.println("Sending request: ");
    mySerial.println(httpRequest);

    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
    mySerial.print("status_response: ");
    mySerial.println(status_response);
 
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) {
      Serial.println("Found Header End. Start Parsing.");
      mySerial.println("Found Header End. Start Parsing.");
    }
 
    parseInfo(client); 
  }
  else {
    Serial.println(" connection failed!");
    mySerial.println(" connection failed!");
  }   
  //断开客户端与服务器连接工作
  client.stop(); 
}

void parseInfo(WiFiClient client){
    String info_name_str;
    bool d3_bool;

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

//************************************************************************//
// https://arduinojson.org/v6/assistant/
/*
  Step 1: Configuration
  Processor:    ESP8266
  Mode:         Deserialize
  Output type:  String
  This is the Assistant for ArduinoJson 6.18.0. 
  Make sure the same version is installed on your computer.
 */
// String input;
StaticJsonDocument<384> doc;
DeserializationError error = deserializeJson(doc, client);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  mySerial.print(F("deserializeJson() failed: "));
  mySerial.println(error.f_str());
  return;
}

JsonObject info = doc["info"];
if(info)
{
Serial.println("Server Json has info : true");
mySerial.println("Server Json has info : true");
const char* info_name = info["name"]; // "taichimaker"
const char* info_url = info["url"]; // "www.taichi-maker.com"
const char* info_email = info["email"]; // "taichimaker@163.com"
info_name_str = info["name"].as<String>();
Serial.print("info_name_str = ");Serial.println(info_name_str);
mySerial.print("info_name_str = ");mySerial.println(info_name_str);
}
else
{
Serial.println("Server Json has info : false");
mySerial.println("Server Json has info : false");
}

JsonObject digital_pin = doc["digital_pin"];
if(digital_pin)
{
Serial.println("Server Json has digital_pin : true");
mySerial.println("Server Json has digital_pin : true");
const char* digital_pin_d1 = digital_pin["d1"]; // "1"
const char* digital_pin_d2 = digital_pin["d2"]; // "0"
const char* digital_pin_d3 = digital_pin["d3"]; // "1"
d3_bool = digital_pin["d3"].as<int>();
Serial.print("d3_bool = ");Serial.println(d3_bool);
mySerial.print("d3_bool = ");mySerial.println(d3_bool);
}
else
{
Serial.println("Server Json has digital_pin : false");
mySerial.println("Server Json has digital_pin : false");
}

const char* analog_pin_a0 = doc["analog_pin"]["a0"]; // "500"
if(analog_pin_a0)
{
    Serial.println("Server Json has analog_pin_a0: true");
    mySerial.println("Server Json has analog_pin_a0: true");
}
else
{
    Serial.println("Server Json has analog_pin_a0: false");
    mySerial.println("Server Json has analog_pin_a0: false");
}
//************************************************************************//

  d3_bool == 0 ? digitalWrite (LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);
}