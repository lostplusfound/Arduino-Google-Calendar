#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecureBearSSL.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <EEPROM.h>
#include <time.h>
const char * WIFI_SSID = "";
const char * WIFI_PASS = "";
const char * AUTHENTICATION_LINK = "";
const char * REQUEST_LINK = "";
const char * GEOLOCATION_LINK = "";
const char * REFRESH_LINK = "";
TFT_eSPI tft = TFT_eSPI();
String access_token;
String refresh_token;
String timestamp;
String date;
time_t timeNow;
int rawOffset;
String getToken();
String getRequest(String &access_token);
String toRFC(time_t timeNow, int offset);
String getTime();
String toFormatString(time_t timeNow);
time_t getTimeUnix();
int getRawOffset();
String refresh(String &refresh_token);
String readStr(int addr);
void putStr(int addr, String &str);
void login();