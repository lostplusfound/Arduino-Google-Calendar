void login() {
  JSONVar token;
  if(EEPROM.read(0) == 127) {
    Serial.println(F("Found refresh in EEPROM"));
    refresh_token = readStr(1);
    Serial.println(refresh_token);
    access_token = refresh(refresh_token);
    Serial.println(access_token);
  } 
  if(access_token.length() == 0 || EEPROM.read(0) != 127) {
    Serial.println(F("Invalid refresh in EEPROM"));
    tft.fillScreen(TFT_WHITE);
    tft.setCursor(0, 0, 2);
    tft.setTextSize(3);
    tft.println(F("Go to to log in! Press anywhere once logged in."));
    while(digitalRead(D3) == 1) {
      yield();
    }
    token = JSON.parse(getToken());
    if(token.hasOwnProperty("access_token")) {
      access_token = String(token["access_token"]);
      Serial.print(F("Access token: "));
      Serial.println(access_token);
    }
    if(token.hasOwnProperty("refresh_token")) {
      refresh_token = String(token["refresh_token"]);
      Serial.print(F("Refresh token: "));
      Serial.println(refresh_token);
    }
    EEPROM.write(0, 127);
    putStr(1, refresh_token);
    Serial.println(F("Saved refresh in EEPROM"));
  }
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(3);
  tft.println(F("Logged in!"));
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
}
