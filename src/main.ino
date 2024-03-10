unsigned long prevMillis;
int count = 0;
bool backlight = HIGH;
void setup() {
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  digitalWrite(D2, HIGH);
  access_token.reserve(256);
  refresh_token.reserve(128);
  timestamp.reserve(32);
  date.reserve(32);
  Serial.begin(115200);
  prefs.begin("calendar");
  initTFT();
  wifiManager.setDebugOutput(false);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("Calendar", "calendar123");
  login();
  prevMillis = millis() + 30000;
}

void loop() {
  unsigned long nowMillis = millis();
  JSONVar token;
  if(nowMillis - prevMillis > 30000) { // refresh every 30s and print to screen
    initTFT();
    count++;
    timeNow += 30;
    timestamp = toRFC(timeNow, rawOffset);
    token = JSON.parse(getRequest(access_token, timestamp));
    while(!token.hasOwnProperty("items")) {
      login();
      token = JSON.parse(getRequest(access_token, timestamp));
    }
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(4);
    tft.println(toFormatString(timeNow));
    tft.setTextSize(3);
    tft.println(F("Today's events: "));
    for(int i = 0; i < token["items"].length(); i++) {
      date = String((const char *) token["items"][i]["start"]["dateTime"]);
      if(timestamp.substring(0, 10) == date.substring(0, 10)) {
        tft.print((const char *) token["items"][i]["summary"]);
        tft.print(F(" at: "));
        tft.println(date.substring(11, 16));
      }
    }
    prevMillis = nowMillis;
  }
  if(count >= 20) { // refresh time every 10 min
    refreshTime();
    count = 0;
  }
  if(digitalRead(D3) == 0) { // toggle backlight if touchscreen was pressed
    backlight = !backlight;
    digitalWrite(D2, backlight);
    delay(500);
  }
}

void configModeCallback (WiFiManager *myWiFiManager) {
  initTFT();
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(4);
  tft.println(F("No WiFi!"));
  tft.setTextSize(2);
  tft.println(F("1) Connect to Wifi network \"Calendar\" with password \"calendar123\""));
  tft.println(F("2) Follow prompt to log in to enter WiFi network credentials"));
  tft.println(F("If you are not asked to log in, open the IP 192.168.4.1 in a browser"));
}

void initTFT() {
  tft.init();
  tft.setRotation(1);
  tft.setCursor(0, 0, 2);
  tft.setTextColor(TFT_BLACK, TFT_WHITE);  
}
