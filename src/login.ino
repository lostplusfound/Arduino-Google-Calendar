void login() {
  JSONVar token;
  initTFT();
  refresh_token = prefs.getString("refresh_token", "");
  if(refresh_token.length() > 0) {
    access_token = refresh(refresh_token);
  } 
  if(access_token.length() == 0) {
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
    token = JSON.parse(getToken());
    if(token.hasOwnProperty("access_token")) {
      access_token = String(token["access_token"]);
    }
    if(token.hasOwnProperty("refresh_token")) {
      refresh_token = String(token["refresh_token"]);
    }
    prefs.putString("refresh_token", refresh_token);
  }
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(0, 0, 2);
  tft.setTextSize(3);
  tft.println(F("Logged in!"));
  refreshTime();
}
