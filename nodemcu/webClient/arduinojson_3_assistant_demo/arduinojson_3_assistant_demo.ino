/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : arduinojson_3_assistant_demo
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200514
程序目的/Purpose          : 
本实例用于演示如何使用ArduinoJson Assistant在线工具生成JSON解析代码。

步骤详解：
1. 使用Serial.begin指令启动串口通讯（此操作用于通过串口监视器检查解析结果）
2. 将需要解析的JSON数据文件复制粘贴到ArduinoJson Assistant在线工具
   https://arduinojson.org/v6/assistant/
3. 将Parsing代码复制粘贴到IDE中
4. 从Parsing代码中找到需要解析的数据所对应的const char*
5. 利用as函数需要解析的数据const char*转换为需要的格式
6. 为了代码精简，可删除无需解析的数据所对应的parsing代码部分

此代码解析的JSON：
{
  "results": [
    {
      "location": {
        "name": "Beijing",
        "country": "CN"
      },
      "now": {
        "text": "Clear",
        "code": "1",
        "temperature": "3"
      },
      "last_update": "2020-03-01T20:10:00+08:00"
    }
  ]
}
***********************************************************************/

#include <ArduinoJson.h>


// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27



void setup() {
  // put your setup code here, to run once:

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  delay(100);
  mySerial.println("");

String input = "{\"results\":[{\"location\":{\"name\":\"Beijing\",\"country\":\"CN\"},\"now\":{\"text\":\"Clear\",\"code\":\"1\",\"temperature\":\"3\"},\"last_update\":\"2020-03-01T20:10:00+08:00\"}]}";

StaticJsonDocument<384> doc;

DeserializationError error = deserializeJson(doc, input);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());

  mySerial.print(F("deserializeJson() failed: "));
  mySerial.println(error.f_str());
  
  return;
}

JsonObject results_0 = doc["results"][0];

const char* results_0_location_name = results_0["location"]["name"]; // "Beijing"
const char* results_0_location_country = results_0["location"]["country"]; // "CN"

JsonObject results_0_now = results_0["now"];
const char* results_0_now_text = results_0_now["text"]; // "Clear"
const char* results_0_now_code = results_0_now["code"]; // "1"
const char* results_0_now_temperature = results_0_now["temperature"]; // "3"

const char* results_0_last_update = results_0["last_update"]; // "2020-03-01T20:10:00+08:00"

String location_name_String = results_0["location"]["name"].as<String>();
int now_temperature_int = results_0_now["temperature"].as<int>();

mySerial.println(location_name_String);
mySerial.println(now_temperature_int);

}

void loop() {
  // put your main code here, to run repeatedly:

}
