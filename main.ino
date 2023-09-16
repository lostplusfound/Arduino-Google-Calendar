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
  EEPROM.begin(4096);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) { 
    yield();
  }
  Serial.println(WiFi.localIP());
  delay(250);
  while(tft.getRotation() != 1) {
    yield();
    tft.init();
    tft.setRotation(1);
    tft.setCursor(0, 0, 2);
    tft.setTextColor(TFT_BLACK, TFT_WHITE);  
  }
  login();
  timeNow = getTimeUnix();
  while(timeNow == -1) {
    delay(100);
    timeNow = getTimeUnix();
  }
  rawOffset = getRawOffset();
  while(rawOffset == -1) {
    delay(100);
    rawOffset = getRawOffset();
  }
  timeNow += rawOffset;
  prevMillis = millis() + 30000;
}

void loop() {
  unsigned long nowMillis = millis();
  JSONVar token;
  if(nowMillis - prevMillis > 30000) { // get events, print to screen
    count++;
    timeNow += 30;
    timestamp = toRFC(timeNow, rawOffset);
    Serial.println(timestamp);
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
      Serial.println(token["items"][i]["summary"]);
      if(timestamp.substring(0, 10) == date.substring(0, 10)) {
        tft.print((const char *) token["items"][i]["summary"]);
        tft.print(F(" at: "));
        tft.println(date.substring(11, 16));
      }
    }
    Serial.println(ESP.getFreeHeap());
    prevMillis = nowMillis;
  }
  if(count >= 20) { // sync time every 10 min
    timeNow = getTimeUnix();
    while(timeNow == -1) {
      delay(100);
      timeNow = getTimeUnix();
    }
    rawOffset = getRawOffset();
    while(rawOffset == -1) {
      delay(100);
      rawOffset = getRawOffset();
    }
    timeNow += rawOffset;
    count = 0;
  }
  if(digitalRead(D3) == 0) { // detect if touchscreen has been pressed
    backlight = !backlight;
    digitalWrite(D2, backlight);
    delay(500);
  }
}