/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : arduinojosn_1_object
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200424
程序目的/Purpose          : 
此程序用于演示如何使用arduinojson库解析以下json信息。该json包含一个对象，
对象中有一个数据。
{
  "name": "taichi-maker",
  "number": 1
}
-----------------------------------------------------------------------
本示例程序为太极创客团队制作的《零基础入门学用物联网》中示例程序。
该教程为对物联网开发感兴趣的朋友所设计和制作。如需了解更多该教程的信息，请参考以下网页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/
***********************************************************************/
#include <ArduinoJson.h>
 
void setup() {
  Serial.begin(9600);
  Serial.println("");
 
  // 重点1：DynamicJsonDocument对象
  const size_t capacity = JSON_OBJECT_SIZE(2) + 30;
  DynamicJsonDocument doc(capacity);
 
  // 重点2：即将解析的json文件
  String json = "{\"name\":\"taichi-maker\",\"number\":1}";
  
  // 重点3：反序列化数据
  deserializeJson(doc, json);
 
  // 重点4：获取解析后的数据信息
  String nameStr = doc["name"].as<String>();
  int numberInt = doc["number"].as<int>();
 
  // 通过串口监视器输出解析后的数据信息
  Serial.print("nameStr = ");Serial.println(nameStr);
  Serial.print("numberInt = ");Serial.println(numberInt);
}
 
void loop() {}