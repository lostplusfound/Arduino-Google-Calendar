unsigned long prevMillis;
bool backlight = HIGH;
void setup() {
  pinMode(D2, OUTPUT);
  pinMode(D3, INPUT);
  digitalWrite(D2, HIGH);
  initTFT();
  wifiManager.setDebugOutput(false);
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("Calendar", "calendar123");
  waitForSync();
  myTZ.setLocation(getTimeZone());
  login();
  prevMillis = millis() + 30000;
}

void loop() {
  unsigned long nowMillis = millis();
  if(nowMillis - prevMillis > 30000) { // refresh every 30s and print to screen
    initTFT();
    String dateTimeNow = myTZ.dateTime(RFC3339);
    String dateNow = dateTimeNow.substring(0, 10);
    if(!calendar.refreshEvents(3, dateTimeNow)) login();
    JSONVar events = calendar.events;
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(4);
    tft.println(myTZ.dateTime("H:i"));
    tft.println(myTZ.dateTime("D, M j Y"));
    tft.setTextSize(3);
    tft.println(F("Today's events: "));
    for(int i = 0; i < events["items"].length(); i++) {
      String startDateTime = String((const char *) events["items"][i]["start"]["dateTime"]);
      String startDate = startDateTime.substring(0, 10);
      if(dateNow == startDate) {
        String startTime = startDateTime.substring(11, 16);
        tft.print((const char *) events["items"][i]["summary"]);
        tft.print(F(" at: "));
        tft.println(startTime);
      }
    }
    prevMillis = nowMillis;
  }
  if(digitalRead(D3) == 0) { // toggle backlight if touchscreen was pressed
    backlight = !backlight;
    digitalWrite(D2, backlight);
    delay(500);
  }
}

void login() {
  initTFT();
  if(WiFi.status() != WL_CONNECTED) {
    wifiManager.autoConnect("Calendar", "calendar123");
  } else if(!calendar.refreshAccessToken()) {
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(4);
    tft.println(F("Not logged in!"));
    tft.setTextSize(2);
    tft.println(F("1) Go to https://tinyurl.com/arduinocalendar and log in with your Google Account"));
    tft.println(F("2) Press anywhere once logged in"));
    while(digitalRead(D3) == 1) {
      yield();
    }
    calendar.getAccessToken();
  }
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(3);
  tft.println(F("Logged in!"));
}

String getTimeZone() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  http.begin(client, GEOIP_URI);
  int code = http.GET();
  JSONVar time = JSON.parse(http.getString());
  if(code > 0) {
    if(time.hasOwnProperty("timezone")) return String(time["timezone"]);
  }
  return "";
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
