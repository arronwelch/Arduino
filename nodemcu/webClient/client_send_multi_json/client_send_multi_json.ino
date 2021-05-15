/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : client_send_multi_json
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : Dapenson
日期/Date（YYYYMMDD）     : 20200425
程序目的/Purpose          : 
本实例用于演示esp8266的json数据通讯。
操作测试本程序需要使用两台8266开发板。其中一台为服务器端，一台为客户端

本程序为客户端程序，功能如下：
1. 实时读取A0、 D1、D2以及D3引脚的读数。
2. 向服务器发送多种Json信息。
3. 在有些发送的信息中包含有D3引脚状态从而控制服务器开发板上
   的LED点亮或熄灭
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200521      CYNO朔           001        调整以使程序与csj_client一致
-----------------------------------------------------------------------
本示例程序为太极创客团队制作的《零基础入门学用物联网》中示例程序。
该教程为对物联网开发感兴趣的朋友所设计和制作。如需了解更多该教程的信息，请参考以下网页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-nodemcu-web-client/http-request/
***********************************************************************/
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>

ESP8266WiFiMulti wifiMulti;           // 建立ESP8266WiFiMulti对象
 
const char* host = "192.168.31.250"; // 网络服务器地址
const int httpPort = 80;            // http端口80

void setup(){
  Serial.begin(9600);          
  Serial.println("");
  
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
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
}
 
void loop(){
  // 发送HTTP请求，使用参数控制函数发送不同类型JSON
  httpRequest(1);    
  delay(2000);
  httpRequest(2); 
  delay(2000); 
  httpRequest(3); 
  delay(2000);   
}
 
// 向服务器发送HTTP请求，请求信息中包含json信息
void httpRequest(int jsonType){
  // 建立WiFi客户端对象，对象名称client
  WiFiClient client;    

  // 根据jsonType参数建立不同类型JSON
  String payloadJson = buildJson(jsonType); 
 
  // 建立字符串，用于HTTP请求
  String httpRequest =  String("GET /") + " HTTP/1.1\r\n" +
                        "Host: " + host + "\r\n" +
                        "Connection: close\r\n\r\n" + payloadJson;
  
  // 通过串口输出连接服务器名称以便查阅连接服务器的网址                      
  Serial.print("Connecting to "); 
  Serial.print(host); 

  if (client.connect(host, httpPort)){ 
    Serial.println(" Success!");            // 连接成功后串口输出“Success”信息
    
    client.print(httpRequest);              // 向服务器发送请求
    Serial.println("Sending request: ");    // 通过串口输出HTTP请求信息内容以便查阅
    Serial.println(httpRequest);     
    
    Serial.println("Web Server Response:"); // 通过串口监视输出服务器响应信息        
    while (client.connected() || client.available()){ 
      if (client.available()){
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    } 
  } else{    // 如果连接不成功则通过串口输出“连接失败”信息
    Serial.println(" failed!");
  } 
  
  client.stop();                      // 断开与服务器的连接
  Serial.print("Disconnected from "); // 并且通过串口输出断开连接信息
  Serial.println(host);   
}

//建立json信息
String buildJson(int type){

  // 开始ArduinoJson Assistant的serialize代码 
  const size_t capacity = JSON_OBJECT_SIZE(1) + 3*JSON_OBJECT_SIZE(3)+140;
  DynamicJsonDocument doc(capacity);
  
  if (type == 1){
    JsonObject info = doc.createNestedObject("info");
    info["name"] = "taichimaker";
    info["url"] = "www.taichi-maker.com";
    info["email"] = "taichimaker@163.com";
  }

  if (type == 2){
    JsonObject digital_pin = doc.createNestedObject("digital_pin");
    digital_pin["d1"] = String(digitalRead(D1));
    digital_pin["d2"] = String(digitalRead(D2));
    digital_pin["d3"] = String(digitalRead(D3));
  }

  if (type == 3){
    JsonObject analog_pin = doc.createNestedObject("analog_pin");
    analog_pin["a0"] = String(analogRead(A0));
  }
  // 结束assistant的serialize代码

  String jsonCode;  
  serializeJson(doc, jsonCode);
  Serial.print("json Code: ");Serial.println(jsonCode); 
   
  return jsonCode;
}
