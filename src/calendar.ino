#include <GoogleCalendarClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecureBearSSL.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <WiFiManager.h>
#include <ezTime.h>

String AUTH_URI = "YOUR AUTH URI";
String API_KEY = "YOUR API KEY";
String CLIENT_ID = "YOUR CLIENT ID";
String CLIENT_SECRET = "YOUR CLIENT SECRET";
String GEOIP_URI = "https://worldtimeapi.org/api/ip";
TFT_eSPI tft = TFT_eSPI();
GoogleCalendarClient calendar(AUTH_URI, API_KEY, CLIENT_ID, CLIENT_SECRET);
WiFiManager wifiManager;
Timezone myTZ;
void login();
String getTimeZone();
void configModeCallback(WiFiManager *myWiFiManager);
void initTFT();
