
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

void setup()
{
    Serial.begin(9600);

  //通过addAp函数存储  WiFi名称       WiFi密码
  wifiMulti.addAP("ssid_from_AP_1", "your_password_for_AP_1"); // 将需要连接的一系列WiFi ID和密码输入这里
  wifiMulti.addAP("ssid_from_AP_2", "your_password_for_AP_2"); // ESP8266-NodeMCU再启动后会扫描当前网络
  wifiMulti.addAP("ssid_from_AP_3", "your_password_for_AP_3"); // 环境查找是否有这里列出的WiFi ID。如果有
//Serial.println("Connecting ...");                            // 则尝试使用此处存储的密码进行连接。

    Serial.print("Connecting ... ");

    int i =0;
    while(wifiMulti.run() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(i++);
        Serial.print(' ');
    }

    Serial.println('\n');
    Serial.println("Connection to ");
    Serial.print(WiFi.SSID());
    Serial.println("IP address:\t");
    Serial.println(WiFi.localIP());
}

void loop()
{

}
