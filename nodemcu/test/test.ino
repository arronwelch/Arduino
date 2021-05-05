
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>

ESP8266WiFiMulti wifiMulti;

ESP8266WebServer esp8266_server(80);

void setup(void)
{
    Serial.begin(9600);
    pinMode(LED_BUILTIN,OUTPUT);

    wifiMulti.addAP("TP-LINK_68EFEC","18163676911*");

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
