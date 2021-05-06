
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer esp8266_server(80);

void setup(void)
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN,OUTPUT);

  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
//Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。

    int i = 0;
    while(wifiMulti.run() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(i++);
        Serial.print(' ');
        if(65535 == i) break;
    }

    Serial.println('\n');
    Serial.print("Connected to ");
    Serial.println(WiFi.SSID());
    Serial.print("IP address:\t");
    Serial.println(WiFi.localIP());

    esp8266_server.begin();
    esp8266_server.on("/",HTTP_GET,handleRoot);
    esp8266_server.on("/LED",HTTP_POST,handleLED);
    esp8266_server.onNotFound(handleNotFound);

    Serial.println("HTTP esp8266_server started");
}


void loop(void)
{
    esp8266_server.handleClient();
}

void handleRoot()
{
    esp8266_server.send(200,"text/html","<form action=\"/LED\" method=\"POST\"> <input type=\"submit\" value = \"Toggle LED\"> </form>");
}


void handleLED()
{
    digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
    esp8266_server.sendHeader("Location","/");//jump setting
    esp8266_server.send(303);//do jump
}

void handleNotFound(){
  esp8266_server.send(404, "text/plain", "404: Not found"); // 发送 HTTP 状态 404 (未找到页面) 并向浏览器发送文字 "404: Not found"
}
