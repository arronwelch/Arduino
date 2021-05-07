/**********************************************************************
项目名称/Project           : 零基础入门学用物联网
程序名称/Program name      : file_stream_demo
团队/Team                 : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author               : CYNO 朔 
日期/Date（YYYYMMDD）      : 20200506
程序目的/Purpose           : 
通过File对象演示Stream操作
***********************************************************************/
#include <FS.h>  

// include the SoftwareSerial library so you can use its functions:
#include <SoftwareSerial.h>
#define rxPin 4
#define txPin 5
//Arduino 1.8.13
//esp8266 Board 2.7.4
//new NodeMcu V3 LOLin(CH340G driver)
//Saleae Logic 2.3.27
 
String file_name = "/taichi-maker/notes.txt"; //被读取的文件位置和名称
 
void setup() {
//   Serial.begin(9600);
//   Serial.println("");

  // define pin modes for tx, rx:
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  // set up a new serial port
  SoftwareSerial mySerial(rxPin, txPin);  
  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);

  // 启动SPIFFS
  if(SPIFFS.begin()){ 
    mySerial.println("SPIFFS Started.");
  } else {
    mySerial.println("SPIFFS Failed to Start.");
  }
 
  File dataFile = SPIFFS.open(file_name, "w");// 建立File对象用于向SPIFFS中的file对象（即/notes.txt）写入信息
  dataFile.println("Hello IOT World.");       // 向dataFile写入字符串信息
  dataFile.close();                           // 完成文件写入后关闭文件
  mySerial.println("Finished Writing data:\"Hello IOT World.\" to SPIFFS");
 
  // 使用find函数从dataFile中找到指定信息
  mySerial.print("Try to find IOT in file:");mySerial.println(file_name); 
   
  dataFile = SPIFFS.open(file_name, "r");     // 以“r”模式再次打开闪存文件
  if (dataFile.find("IOT")){                  // 在闪存文件中查找文字"IOT"
    mySerial.print("Found IOT in file: ");      // 如果找到则告知用户找到文字"IOT"
    mySerial.println(file_name);         
  }
 
  // 使用readString读取执行完find函数后的dataFile内容并显示与串口监视器
  mySerial.println("Use readString to get contents of dataFile after find");
  mySerial.println(dataFile.readString());
    
  dataFile.close();                           // 完成操作后关闭文件  
}
 
void loop() {}