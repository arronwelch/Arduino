/**********************************************************************
项目名称/Project          : 零基础入门学用物联网
程序名称/Program name     : weather_forecast
团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
作者/Author              : CYNO朔
日期/Date（YYYYMMDD）     : 20200601
程序目的/Purpose          : 
通过心知天气(www.seniverse.com)免费服务获取天气预报信息（三日）。
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
    String reqRes = "/v3/weather/daily.json?key=" + reqUserKey +
                    +"&location=" + reqLocation + "&language=en&unit=" +
                    reqUnit + "&start=0&days=3";

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

        //  String serverJson = client.readString();
        //  Serial.println(serverJson);

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
//     const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_ARRAY_SIZE(3) + JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 3 * JSON_OBJECT_SIZE(14) + 860;

//     DynamicJsonDocument doc(capacity);

//     deserializeJson(doc, client);

//     JsonObject results_0 = doc["results"][0];

//     JsonArray results_0_daily = results_0["daily"];

//     JsonObject results_0_daily_0 = results_0_daily[0];
//     const char *results_0_daily_0_date = results_0_daily_0["date"];
//     const char *results_0_daily_0_text_day = results_0_daily_0["text_day"];
//     const char *results_0_daily_0_code_day = results_0_daily_0["code_day"];
//     const char *results_0_daily_0_text_night = results_0_daily_0["text_night"];
//     const char *results_0_daily_0_code_night = results_0_daily_0["code_night"];
//     const char *results_0_daily_0_high = results_0_daily_0["high"];
//     const char *results_0_daily_0_low = results_0_daily_0["low"];
//     const char *results_0_daily_0_rainfall = results_0_daily_0["rainfall"];
//     const char *results_0_daily_0_precip = results_0_daily_0["precip"];
//     const char *results_0_daily_0_wind_direction = results_0_daily_0["wind_direction"];
//     const char *results_0_daily_0_wind_direction_degree = results_0_daily_0["wind_direction_degree"];
//     const char *results_0_daily_0_wind_speed = results_0_daily_0["wind_speed"];
//     const char *results_0_daily_0_wind_scale = results_0_daily_0["wind_scale"];
//     const char *results_0_daily_0_humidity = results_0_daily_0["humidity"];

//     JsonObject results_0_daily_1 = results_0_daily[1];
//     const char *results_0_daily_1_date = results_0_daily_1["date"];
//     const char *results_0_daily_1_text_day = results_0_daily_1["text_day"];
//     const char *results_0_daily_1_code_day = results_0_daily_1["code_day"];
//     const char *results_0_daily_1_text_night = results_0_daily_1["text_night"];
//     const char *results_0_daily_1_code_night = results_0_daily_1["code_night"];
//     const char *results_0_daily_1_high = results_0_daily_1["high"];
//     const char *results_0_daily_1_low = results_0_daily_1["low"];
//     const char *results_0_daily_1_rainfall = results_0_daily_1["rainfall"];
//     const char *results_0_daily_1_precip = results_0_daily_1["precip"];
//     const char *results_0_daily_1_wind_direction = results_0_daily_1["wind_direction"];
//     const char *results_0_daily_1_wind_direction_degree = results_0_daily_1["wind_direction_degree"];
//     const char *results_0_daily_1_wind_speed = results_0_daily_1["wind_speed"];
//     const char *results_0_daily_1_wind_scale = results_0_daily_1["wind_scale"];
//     const char *results_0_daily_1_humidity = results_0_daily_1["humidity"];

//     JsonObject results_0_daily_2 = results_0_daily[2];
//     const char *results_0_daily_2_date = results_0_daily_2["date"];
//     const char *results_0_daily_2_text_day = results_0_daily_2["text_day"];
//     const char *results_0_daily_2_code_day = results_0_daily_2["code_day"];
//     const char *results_0_daily_2_text_night = results_0_daily_2["text_night"];
//     const char *results_0_daily_2_code_night = results_0_daily_2["code_night"];
//     const char *results_0_daily_2_high = results_0_daily_2["high"];
//     const char *results_0_daily_2_low = results_0_daily_2["low"];
//     const char *results_0_daily_2_rainfall = results_0_daily_2["rainfall"];
//     const char *results_0_daily_2_precip = results_0_daily_2["precip"];
//     const char *results_0_daily_2_wind_direction = results_0_daily_2["wind_direction"];
//     const char *results_0_daily_2_wind_direction_degree = results_0_daily_2["wind_direction_degree"];
//     const char *results_0_daily_2_wind_speed = results_0_daily_2["wind_speed"];
//     const char *results_0_daily_2_wind_scale = results_0_daily_2["wind_scale"];
//     const char *results_0_daily_2_humidity = results_0_daily_2["humidity"];

//     const char *results_0_last_update = results_0["last_update"];

//     // 从以上信息中摘选几个通过串口监视器显示
//     String results_0_daily_0_date_str = results_0_daily_0["date"].as<String>();
//     String results_0_daily_0_text_day_str = results_0_daily_0["text_day"].as<String>();
//     int results_0_daily_0_code_day_int = results_0_daily_0["code_day"].as<int>();
//     String results_0_daily_0_text_night_str = results_0_daily_0["text_night"].as<String>();
//     int results_0_daily_0_code_night_int = results_0_daily_0["code_night"].as<int>();
//     int results_0_daily_0_high_int = results_0_daily_0["high"].as<int>();
//     int results_0_daily_0_low_int = results_0_daily_0["low"].as<int>();
//     String results_0_last_update_str = results_0["last_update"].as<String>();

//     Serial.println(F("======Today Weahter ======="));
//     Serial.print(F("DATE: "));
//     Serial.println(results_0_daily_0_date_str);
//     Serial.print(F("Day Weather: "));
//     Serial.print(results_0_daily_0_text_day_str);
//     Serial.print(F(" "));
//     Serial.println(results_0_daily_0_code_day_int);
//     Serial.print(F("Night Weather: "));
//     Serial.print(results_0_daily_0_text_night_str);
//     Serial.print(F(" "));
//     Serial.println(results_0_daily_0_code_night_int);
//     Serial.print(F("High: "));
//     Serial.println(results_0_daily_0_high_int);
//     Serial.print(F("LOW: "));
//     Serial.println(results_0_daily_0_low_int);
//     Serial.print(F("Last Update: "));
//     Serial.println(results_0_last_update_str);
//     Serial.println(F("=============================="));

//     // 从以上信息中摘选几个通过串口监视器显示
//     String results_0_daily_1_date_str = results_0_daily_1["date"].as<String>();
//     String results_0_daily_1_text_day_str = results_0_daily_1["text_day"].as<String>();
//     int results_0_daily_1_code_day_int = results_0_daily_1["code_day"].as<int>();
//     String results_0_daily_1_text_night_str = results_0_daily_1["text_night"].as<String>();
//     int results_0_daily_1_code_night_int = results_0_daily_1["code_night"].as<int>();
//     int results_0_daily_1_high_int = results_0_daily_1["high"].as<int>();
//     int results_0_daily_1_low_int = results_0_daily_1["low"].as<int>();
//     //String results_0_last_update_str = results_0["last_update"].as<String>();

//     Serial.println(F("======Tomorrow Weahter ======="));
//     Serial.print(F("DATE: "));
//     Serial.println(results_0_daily_1_date_str);
//     Serial.print(F("Day Weather: "));
//     Serial.print(results_0_daily_1_text_day_str);
//     Serial.print(F(" "));
//     Serial.println(results_0_daily_1_code_day_int);
//     Serial.print(F("Night Weather: "));
//     Serial.print(results_0_daily_1_text_night_str);
//     Serial.print(F(" "));
//     Serial.println(results_0_daily_1_code_night_int);
//     Serial.print(F("High: "));
//     Serial.println(results_0_daily_1_high_int);
//     Serial.print(F("LOW: "));
//     Serial.println(results_0_daily_1_low_int);
//     Serial.print(F("Last Update: "));
//     Serial.println(results_0_last_update_str);
//     Serial.println(F("=============================="));

//     // 从以上信息中摘选几个通过串口监视器显示
//     String results_0_daily_2_date_str = results_0_daily_2["date"].as<String>();
//     String results_0_daily_2_text_day_str = results_0_daily_2["text_day"].as<String>();
//     int results_0_daily_2_code_day_int = results_0_daily_2["code_day"].as<int>();
//     String results_0_daily_2_text_night_str = results_0_daily_2["text_night"].as<String>();
//     int results_0_daily_2_code_night_int = results_0_daily_2["code_night"].as<int>();
//     int results_0_daily_2_high_int = results_0_daily_2["high"].as<int>();
//     int results_0_daily_2_low_int = results_0_daily_2["low"].as<int>();
//     //String results_0_last_update_str = results_0["last_update"].as<String>();

//     Serial.println(F("======The day after tomorrow Weahter ======="));
//     Serial.print(F("DATE: "));
//     Serial.println(results_0_daily_2_date_str);
//     Serial.print(F("Day Weather: "));
//     Serial.print(results_0_daily_2_text_day_str);
//     Serial.print(F(" "));
//     Serial.println(results_0_daily_2_code_day_int);
//     Serial.print(F("Night Weather: "));
//     Serial.print(results_0_daily_2_text_night_str);
//     Serial.print(F(" "));
//     Serial.println(results_0_daily_2_code_night_int);
//     Serial.print(F("High: "));
//     Serial.println(results_0_daily_2_high_int);
//     Serial.print(F("LOW: "));
//     Serial.println(results_0_daily_2_low_int);
//     Serial.print(F("Last Update: "));
//     Serial.println(results_0_last_update_str);
//     Serial.println(F("=============================="));
// }


// 利用ArduinoJson库解析心知天气响应信息
void parseInfo(WiFiClient client)
{
// String input;

DynamicJsonDocument doc(1536);

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

for (JsonObject elem : results_0["daily"].as<JsonArray>()) {

  const char* date = elem["date"]; // "2021-05-15", "2021-05-16", "2021-05-17"
  const char* text_day = elem["text_day"]; // "Cloudy", "Cloudy", "Thundershower"
  const char* code_day = elem["code_day"]; // "4", "4", "11"
  const char* text_night = elem["text_night"]; // "Cloudy", "Cloudy", "Thundershower"
  const char* code_night = elem["code_night"]; // "4", "4", "11"
  const char* high = elem["high"]; // "32", "33", "33"
  const char* low = elem["low"]; // "27", "26", "26"
  const char* rainfall = elem["rainfall"]; // "0.0", "0.0", "0.0"
  const char* precip = elem["precip"]; // "0", "0", "0"
  const char* wind_direction = elem["wind_direction"]; // "SW", "CLM", "CLM"
  const char* wind_direction_degree = elem["wind_direction_degree"]; // "225", "", ""
  const char* wind_speed = elem["wind_speed"]; // "15.3", "8.4", "3.0"
  const char* wind_scale = elem["wind_scale"]; // "3", "2", "1"
  const char* humidity = elem["humidity"]; // "70", "71", "72"

}

const char* results_0_last_update = results_0["last_update"]; // "2021-05-15T18:00:00+08:00"


int cout = 0;
    // 从以上信息中摘选几个通过串口监视器显示
for (JsonObject elem : results_0["daily"].as<JsonArray>()) {

  const char* date = elem["date"]; // "2021-05-15", "2021-05-16", "2021-05-17"
  const char* text_day = elem["text_day"]; // "Cloudy", "Cloudy", "Thundershower"
  const char* code_day = elem["code_day"]; // "4", "4", "11"
  const char* text_night = elem["text_night"]; // "Cloudy", "Cloudy", "Thundershower"
  const char* code_night = elem["code_night"]; // "4", "4", "11"
  const char* high = elem["high"]; // "32", "33", "33"
  const char* low = elem["low"]; // "27", "26", "26"
  const char* rainfall = elem["rainfall"]; // "0.0", "0.0", "0.0"
  const char* precip = elem["precip"]; // "0", "0", "0"
  const char* wind_direction = elem["wind_direction"]; // "SW", "CLM", "CLM"
  const char* wind_direction_degree = elem["wind_direction_degree"]; // "225", "", ""
  const char* wind_speed = elem["wind_speed"]; // "15.3", "8.4", "3.0"
  const char* wind_scale = elem["wind_scale"]; // "3", "2", "1"
  const char* humidity = elem["humidity"]; // "70", "71", "72"

    String results_0_daily_date_str = elem["date"].as<String>();
    String results_0_daily_text_day_str = elem["text_day"].as<String>();
    int results_0_daily_code_day_int = elem["code_day"].as<int>();
    String results_0_daily_text_night_str = elem["text_night"].as<String>();
    int results_0_daily_code_night_int = elem["code_night"].as<int>();
    int results_0_daily_high_int = elem["high"].as<int>();
    int results_0_daily_low_int = elem["low"].as<int>();

    Serial.print(F("======Weahter "));
    Serial.print(cout);
    Serial.println(F("======="));

    Serial.print(F("DATE: "));
    Serial.println(results_0_daily_date_str);
    Serial.print(F("Day Weather: "));
    Serial.print(results_0_daily_text_day_str);
    Serial.print(F(" "));
    Serial.println(results_0_daily_code_day_int);
    Serial.print(F("Night Weather: "));
    Serial.print(results_0_daily_text_night_str);
    Serial.print(F(" "));
    Serial.println(results_0_daily_code_night_int);
    Serial.print(F("High: "));
    Serial.println(results_0_daily_high_int);
    Serial.print(F("LOW: "));
    Serial.println(results_0_daily_low_int);

    cout++;
}

    String results_0_last_update_str = results_0["last_update"].as<String>();
    Serial.print(F("Last Update: "));
    Serial.println(results_0_last_update_str);

    Serial.println(F("=============================="));

}
