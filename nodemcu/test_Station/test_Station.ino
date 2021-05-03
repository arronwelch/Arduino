
#include <ESP8266WiFi.h>

const char* ssid = "TP-LINK_68EFEC";
const char* password = "18163676911*";

void setup()
{
    Serial.begin(9600);

    WiFi.begin(ssid,password);
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");

    int i =0;
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.print(i++);
        Serial.print(' ');
    }

    Serial.println("");
    Serial.println("Connection established!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop()
{

}
