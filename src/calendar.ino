#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecureBearSSL.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <time.h>
#include <WiFiManager.h>
#include <Preferences.h>
const String AUTHENTICATION_LINK = "";
const String API_KEY = "";
const String REQUEST_LINK = "https://www.googleapis.com/calendar/v3/calendars/primary/events?key=" + API_KEY;
const String GEOLOCATION_LINK = "https://worldtimeapi.org/api/ip";
const String REFRESH_LINK = "https://oauth2.googleapis.com/token";
const String CLIENT_ID = "";
const String CLIENT_SECRET = "";
TFT_eSPI tft = TFT_eSPI();
WiFiManager wifiManager;
Preferences prefs;
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
void refreshTime();
String refresh(String &refresh_token);
void login();
void configModeCallback(WiFiManager *myWiFiManager);
void initTFT();
