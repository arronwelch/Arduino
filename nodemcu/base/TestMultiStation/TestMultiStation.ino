
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti wifiMulti;

void setup()
{
    Serial.begin(9600);

    wifiMulti.addAP("TP-LINK_68EFEC","18163676911*");
    wifiMulti.addAP("TP-LINK_DB58","nch13787252353");

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
