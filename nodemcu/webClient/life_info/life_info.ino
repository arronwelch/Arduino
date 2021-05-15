/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : life_info
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200602
程序目的/Purpose          : 
通过心知天气(www.seniverse.com)免费服务获取生活指数信息。
-----------------------------------------------------------------------
其它说明 / Other Description
心知天气API文档说明: https://www.seniverse.com/docs

本程序为太极创客团队制作的免费视频教程《零基础入门学用物联网 》中一部分。该教程系统的
向您讲述ESP8266的物联网应用相关的软件和硬件知识。以下是该教程目录页：
http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/                                
***********************************************************************/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char *ssid = "Xiaomi_FD94";     // 连接WiFi名（此处使用taichi-maker为示例）
                                      // 请将您需要连接的WiFi名填入引号中
const char *password = "LuckDuck2^8"; // 连接WiFi密码（此处使用12345678为示例）
                                      // 请将您需要连接的WiFi密码填入引号中

const char *host = "api.seniverse.com"; // 将要连接的服务器地址
const int httpPort = 80;                // 将要连接的服务器端口

// 心知天气HTTP请求所需信息
String reqUserKey = "SD2btdfbH35ybOn4w"; // 私钥
String reqLocation = "Shenzhen";         // 城市
String reqUnit = "c";                    // 摄氏/华氏

void setup()
{
    Serial.begin(9600);
    Serial.println("");

    // 连接WiFi
    connectWiFi();
}

void loop()
{
    // 建立心知天气API当前天气请求资源地址
    String reqRes = "/v3/life/suggestion.json?key=" + reqUserKey +
                    +"&location=" + reqLocation +
                    "&language=en";

    // 向心知天气服务器服务器请求信息并对信息进行解析
    httpRequest(reqRes);
    delay(3000);
}

// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequest(String reqRes)
{
    WiFiClient client;

    // 建立http请求信息
    String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                         "Host: " + host + "\r\n" +
                         "Connection: close\r\n\r\n";
    Serial.println("");
    Serial.print("Connecting to ");
    Serial.print(host);

    // 尝试连接服务器
    if (client.connect(host, 80))
    {
        Serial.println(" Success!");

        // 向服务器发送http请求信息
        client.print(httpRequest);
        Serial.println("Sending request: ");
        Serial.println(httpRequest);

        // 获取并显示服务器响应状态行
        String status_response = client.readStringUntil('\n');
        Serial.print("status_response: ");
        Serial.println(status_response);

        // 使用find跳过HTTP响应头
        if (client.find("\r\n\r\n"))
        {
            Serial.println("Found Header End. Start Parsing.");
        }

        // String serverJson = client.readString();
        // Serial.println(serverJson);

        // 利用ArduinoJson库解析心知天气响应信息
        parseInfo(client);
    }
    else
    {
        Serial.println(" connection failed!");
    }
    //断开客户端与服务器连接工作
    client.stop();
}

// 连接WiFi
void connectWiFi()
{
    WiFi.begin(ssid, password);     // 启动网络连接
    Serial.print("Connecting to "); // 串口监视器输出网络连接信息
    Serial.print(ssid);
    Serial.println(" ..."); // 告知用户NodeMCU正在尝试WiFi连接

    int i = 0; // 这一段程序语句用于检查WiFi是否连接成功
    while (WiFi.status() != WL_CONNECTED)
    {                // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
        delay(1000); // 如果WiFi连接成功则返回值为WL_CONNECTED
        Serial.print(i++);
        Serial.print(' ');                     // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
    }                                          // 同时NodeMCU将通过串口监视器输出连接时长读秒。
                                               // 这个读秒是通过变量i每隔一秒自加1来实现的。
    Serial.println("");                        // WiFi连接成功后
    Serial.println("Connection established!"); // NodeMCU将通过串口监视器输出"连接成功"信息。
    Serial.print("IP address:    ");           // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
    Serial.println(WiFi.localIP());            // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。
}

// // 利用ArduinoJson库解析心知天气响应信息
// void parseInfo(WiFiClient client)
// {
//     const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 6 * JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + 2 * JSON_OBJECT_SIZE(6) + 400;
//     DynamicJsonDocument doc(capacity);

//     deserializeJson(doc, client);

//     JsonObject results_0 = doc["results"][0];

//     JsonObject results_0_suggestion = results_0["suggestion"];

//     const char *results_0_suggestion_car_washing_brief = results_0_suggestion["car_washing"]["brief"];
//     const char *results_0_suggestion_car_washing_details = results_0_suggestion["car_washing"]["details"];

//     const char *results_0_suggestion_dressing_brief = results_0_suggestion["dressing"]["brief"];
//     const char *results_0_suggestion_dressing_details = results_0_suggestion["dressing"]["details"];

//     const char *results_0_suggestion_flu_brief = results_0_suggestion["flu"]["brief"];
//     const char *results_0_suggestion_flu_details = results_0_suggestion["flu"]["details"];

//     const char *results_0_suggestion_sport_brief = results_0_suggestion["sport"]["brief"];
//     const char *results_0_suggestion_sport_details = results_0_suggestion["sport"]["details"];

//     const char *results_0_suggestion_travel_brief = results_0_suggestion["travel"]["brief"];
//     const char *results_0_suggestion_travel_details = results_0_suggestion["travel"]["details"];

//     const char *results_0_suggestion_uv_brief = results_0_suggestion["uv"]["brief"];
//     const char *results_0_suggestion_uv_details = results_0_suggestion["uv"]["details"];

//     const char *results_0_last_update = results_0["last_update"];

//     // 通过串口监视器显示以上信息
//     String results_0_suggestion_car_washing_brief_str = results_0_suggestion["car_washing"]["brief"];

//     String results_0_suggestion_dressing_brief_str = results_0_suggestion["dressing"]["brief"];

//     String results_0_suggestion_flu_brief_str = results_0_suggestion["flu"]["brief"];

//     String results_0_suggestion_sport_brief_str = results_0_suggestion["sport"]["brief"];

//     String results_0_suggestion_travel_brief_str = results_0_suggestion["travel"]["brief"];

//     String results_0_suggestion_uv_brief_str = results_0_suggestion["uv"]["brief"];

//     const char *results_0_last_update_str = results_0["last_update"];

//     Serial.println(F("======Life Info======="));
//     Serial.print(F("Car Washing: "));
//     Serial.println(results_0_suggestion_car_washing_brief_str);
//     Serial.print(F("Dressing: "));
//     Serial.println(results_0_suggestion_dressing_brief_str);
//     Serial.print(F("Flu: "));
//     Serial.println(results_0_suggestion_flu_brief_str);
//     Serial.print(F("Sport: "));
//     Serial.println(results_0_suggestion_sport_brief_str);
//     Serial.print(F("Travel: "));
//     Serial.println(results_0_suggestion_travel_brief_str);
//     Serial.print(F("UV: "));
//     Serial.println(results_0_suggestion_uv_brief_str);
//     Serial.print(F("Last Update: "));
//     Serial.println(results_0_last_update_str);
//     Serial.println(F("======================"));
// }


// 利用ArduinoJson库解析心知天气响应信息
void parseInfo(WiFiClient client)
{

// String input;

StaticJsonDocument<1024> doc;

DeserializationError error = deserializeJson(doc, client);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

JsonObject results_0 = doc["results"][0];

JsonObject results_0_location = results_0["location"];
const char* results_0_location_id = results_0_location["id"]; // "WS10730EM8EV"
const char* results_0_location_name = results_0_location["name"]; // "Shenzhen"
const char* results_0_location_country = results_0_location["country"]; // "CN"
const char* results_0_location_path = results_0_location["path"]; // "Shenzhen,Shenzhen,Guangdong,China"
const char* results_0_location_timezone = results_0_location["timezone"]; // "Asia/Shanghai"
const char* results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"

JsonObject results_0_suggestion = results_0["suggestion"];

const char* results_0_suggestion_car_washing_brief = results_0_suggestion["car_washing"]["brief"];
const char* results_0_suggestion_car_washing_details = results_0_suggestion["car_washing"]["details"];

const char* results_0_suggestion_dressing_brief = results_0_suggestion["dressing"]["brief"]; // "Very ...
const char* results_0_suggestion_dressing_details = results_0_suggestion["dressing"]["details"]; // ""

const char* results_0_suggestion_flu_brief = results_0_suggestion["flu"]["brief"]; // "Rarely"
const char* results_0_suggestion_flu_details = results_0_suggestion["flu"]["details"]; // ""

const char* results_0_suggestion_sport_brief = results_0_suggestion["sport"]["brief"]; // "Not Good"
const char* results_0_suggestion_sport_details = results_0_suggestion["sport"]["details"]; // ""

const char* results_0_suggestion_travel_brief = results_0_suggestion["travel"]["brief"]; // "Good"
const char* results_0_suggestion_travel_details = results_0_suggestion["travel"]["details"]; // ""

const char* results_0_suggestion_uv_brief = results_0_suggestion["uv"]["brief"]; // "Moderate"
const char* results_0_suggestion_uv_details = results_0_suggestion["uv"]["details"]; // ""

const char* results_0_last_update = results_0["last_update"]; // "2021-05-15T18:31:37+08:00"


    // 通过串口监视器显示以上信息
    String results_0_suggestion_car_washing_brief_str = results_0_suggestion["car_washing"]["brief"];

    String results_0_suggestion_dressing_brief_str = results_0_suggestion["dressing"]["brief"];

    String results_0_suggestion_flu_brief_str = results_0_suggestion["flu"]["brief"];

    String results_0_suggestion_sport_brief_str = results_0_suggestion["sport"]["brief"];

    String results_0_suggestion_travel_brief_str = results_0_suggestion["travel"]["brief"];

    String results_0_suggestion_uv_brief_str = results_0_suggestion["uv"]["brief"];

    const char *results_0_last_update_str = results_0["last_update"];

    Serial.println(F("======Life Info======="));
    Serial.print(F("Car Washing: "));
    Serial.println(results_0_suggestion_car_washing_brief_str);
    Serial.print(F("Dressing: "));
    Serial.println(results_0_suggestion_dressing_brief_str);
    Serial.print(F("Flu: "));
    Serial.println(results_0_suggestion_flu_brief_str);
    Serial.print(F("Sport: "));
    Serial.println(results_0_suggestion_sport_brief_str);
    Serial.print(F("Travel: "));
    Serial.println(results_0_suggestion_travel_brief_str);
    Serial.print(F("UV: "));
    Serial.println(results_0_suggestion_uv_brief_str);
    Serial.print(F("Last Update: "));
    Serial.println(results_0_last_update_str);
    Serial.println(F("======================"));

}
