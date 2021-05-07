/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : stream_find_parseInt
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200308
程序目的/Purpose          : 
此程序使用Serial库来演示Stream类中的parseInt函数的使用方法。parseInt函数可用于从设备
接收到的数据中寻找整数数值。当您把本程序上传后，可通过串口监视器进行以下测试。
 
实验：输入信息：
k123oktaichi678maker567
设备输出信息:
Please enter input...
Found ok in user input.
serialParseInt = 678
serialInput = maker567
-----------------------------------------------------------------------
本示例程序为太极创客团队制作的《零基础入门学用物联网》中示例程序。
该教程为对物联网开发感兴趣的朋友所设计和制作。如需了解更多该教程的信息，请参考以下网页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/iot-c/esp8266-nodemcu-web-client/http-request/
***********************************************************************/

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27

void setup() {
  Serial.begin(9600);
  //Serial.println("");
  //Serial.println("Please enter input...");

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  mySerial.println("");
  mySerial.println("Please enter input...");  
}
 
void loop() {

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  while(Serial.available()){
    if(Serial.find("ok")){
      mySerial.println("Found ok in user input.");
 
      int serialParseInt = Serial.parseInt();
      mySerial.print("serialParseInt = ");
      mySerial.println(serialParseInt);
      
      String serialInput = Serial.readString();
      mySerial.print("serialInput = ");
      mySerial.println(serialInput);
    }
  }
}