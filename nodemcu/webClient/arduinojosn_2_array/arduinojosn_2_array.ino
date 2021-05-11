/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : arduinojosn_2_array
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200424
程序目的/Purpose          : 
此程序用于演示如何使用arduinojson库解析以下json信息，该json包含一个数组，
数组有两个元素，每个元素都是一个对象，每一个对象都有一个数据。
[
  {
    "name": "taichi-maker"
  },
  {
    "website": "www.taichi-maker.com"
  }
]
-----------------------------------------------------------------------
本示例程序为太极创客团队制作的《零基础入门学用物联网》中示例程序。
该教程为对物联网开发感兴趣的朋友所设计和制作。如需了解更多该教程的信息，请参考以下网页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/
***********************************************************************/

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27

#include <ArduinoJson.h>
 
void setup() {
  Serial.begin(9600);

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(100);
  mySerial.println("");
 
  // 重点1：DynamicJsonDocument对象
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(1) + 60;
  DynamicJsonDocument doc(capacity);
  
  // 重点2：即将解析的json文件
  String json = "[{\"name\":\"taichi-maker\"},{\"website\":\"www.taichi-maker.com\"}]";
 
  // 重点3：反序列化数据
  deserializeJson(doc, json);
 
 
  String nameStr = doc[0]["name"].as<String>();
  String websiteStr = doc[1]["website"].as<String>();
 
  // 通过串口监视器输出解析后的数据信息
  Serial.print("nameStr = ");Serial.println(nameStr);
  Serial.print("websiteStr = ");Serial.println(websiteStr);

  mySerial.print("nameStr = ");mySerial.println(nameStr);
  mySerial.print("websiteStr = ");mySerial.println(websiteStr);

}
 
void loop() {}