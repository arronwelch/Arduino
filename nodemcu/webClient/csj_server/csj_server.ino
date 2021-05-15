/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : csj_server
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200425
程序目的/Purpose          : 
本实例用于演示esp8266的json数据通讯。
操作测试本程序需要使用两台8266开发板。其中一台为服务器端，一台为客户端。
本程序为服务器程序，功能如下：

1. 获取客户端请求信息中的json。
2. 解析json信息内容。
3. 将解析后的数据信息显示于串口监视器
4. 利用json中客户端D3引脚（按键引脚）读数来控制服务器端开发板上LED的点亮和熄灭
-----------------------------------------------------------------------
修订历史/Revision History  
日期/Date    作者/Author      参考号/Ref    修订说明/Revision Description
20200520      CYNO朔           001        修改parseInfo部分，使其与cgj_client_2
                                          更加一致
-----------------------------------------------------------------------
本示例程序为太极创客团队制作的《零基础入门学用物联网》中示例程序。
该教程为对物联网开发感兴趣的朋友所设计和制作。如需了解更多该教程的信息，请参考以下网页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/
***********************************************************************/
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>   

ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是'wifiMulti'

IPAddress local_IP(192, 168, 31, 150); // 设置ESP8266-NodeMCU联网后的IP
IPAddress gateway(192, 168, 31, 1);    // 设置网关IP（通常网关IP是WiFI路由IP）
IPAddress subnet(255, 255, 255, 0);   // 设置子网掩码
IPAddress dns(192,168,31,1);           // 设置局域网DNS的IP（通常局域网DNS的IP是WiFI路由IP）
 
// 建立WiFiServerSecure对象
WiFiServer server(80);
 
void setup() {
  Serial.begin(9600);
  Serial.println();
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  // 设置开发板网络环境
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to Config ESP8266 IP"); 
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
  }                                          // 一旦连接WiFI成功，wifiMulti.run()将会返回“WL_CONNECTED”。这也是
                                             // 此处while循环判断是否跳出循环的条件。
  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println('\n');                     // WiFi连接成功后
  Serial.print("Connected to ");            // NodeMCU将通过串口监视器输出。
  Serial.println(WiFi.SSID());              // 连接的WiFI名称
  Serial.print("IP address:\t");            // 以及
  Serial.println(WiFi.localIP());           // NodeMCU的IP地址
 
  // 启动服务器
  server.begin();
}
 
void loop() {
  runServer();  // 运行服务器
}

// 运行服务器
void runServer(){
  // 重点1：建立WiFiClient对象用于处理客户端请求信息
  WiFiClient incomingClient = server.available();

  // 如果没有客户端连接服务器，则“跳过”本函数中后续程序内容
  if (!incomingClient) {
    return;
  }
  
  Serial.println("====Client  Connected===");
  
  // 重点2：如果有客户端连接服务器，则尝试使用find跳过HTTP请求头
  if (incomingClient.find("\r\n\r\n")) {
    Serial.println("Found Header End. Start Parsing.");
  }
  else
  {
      return;
  }
  
  // 解析请求体中的json信息 
  parseInfo(incomingClient);

  // 建立服务器响应信息
  String httpResponse =
        "HTTP/1.0 200 OK\r\n"
        "Connection: close\r\n"
        "Content-Type: text/plain;\r\n"
        "\r\n"
        "client_message_received";

  // 向客户端发送以上服务器响应信息
  incomingClient.print(httpResponse); 

  incomingClient.stop();  
  Serial.println("incomingClient stop");     
}

// 重点3：解析请求体中的json信息
void parseInfo(WiFiClient client){
  bool d3_bool;          // 建立变量存储客户端开发板按键信息
  String info_name_str;  // 建立变量存储字符串信息
  int analog_pin_a0_int; // 建立变量存储客户端开发板模拟输入引脚读数
  
  const size_t capacity = JSON_OBJECT_SIZE(1) + 3*JSON_OBJECT_SIZE(3) + 140;
  DynamicJsonDocument doc(capacity);
  
  deserializeJson(doc, client);
  
  JsonObject info = doc["info"];
  if(info){
    Serial.println("Server Json has info: true");
    const char* info_name = info["name"];
    const char* info_url = info["url"];
    const char* info_email = info["email"];
    info_name_str =  info["name"].as<String>();
    Serial.print("info_name_str = ");Serial.println(info_name_str);
  } else {
    Serial.println("Server Json has info: false");
  }
  
  JsonObject digital_pin = doc["digital_pin"];
  if (digital_pin){
    Serial.println("Server Json has digital_pin: true");
    const char* digital_pin_d1 = digital_pin["d1"]; 
    const char* digital_pin_d2 = digital_pin["d2"]; 
    const char* digital_pin_d3 = digital_pin["d3"];
    d3_bool = digital_pin["d3"].as<int>();
    Serial.print("d3_bool = ");Serial.println(d3_bool);
  } else {
    Serial.println("Server Json has digital_pin: false");
  }

  const char* analog_pin_a0 = doc["analog_pin"]["a0"]; 
  if (analog_pin_a0){
    Serial.println("Server Json has analog_pin_a0: true");
    analog_pin_a0_int = doc["analog_pin"]["a0"].as<int>(); 
    Serial.print("analog_pin_a0 = ");Serial.println(analog_pin_a0);
  } else {
    Serial.println("Server Json has analog_pin_a0: false");
  }
   
  // 利用d3引脚读数控制板上LED点亮和熄灭
  d3_bool == 0 ? digitalWrite(LED_BUILTIN, LOW) : digitalWrite(LED_BUILTIN, HIGH);  
}