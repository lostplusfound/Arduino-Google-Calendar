String toFormatString(time_t timeNow) {;
  struct tm * timer = gmtime(&timeNow);
  char buf[32];
  strftime(buf, sizeof(buf) - 1, "%R%n%a, %b %d %G", timer);
  return String(buf);
}
time_t getTimeUnix() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  http.begin(client, GEOLOCATION_LINK);
  int code = http.GET();
  JSONVar time;
  time = JSON.parse(http.getString());
  if(code > 0) {
    if(time.hasOwnProperty("unixtime")) {
      return (int) time["unixtime"];
    }
  }
  return -1;
}
String toRFC(time_t timeNow, int offset) {
  String formatString;
  offset /= 3600;
  if(offset < 10 && offset > -10) {
    if(offset < 0) {
      formatString = String("%FT%T-0") + String(abs(offset)) + String(":00");
    } else {
      formatString = String("%FT%T+0") + String(offset) + String(":00");
    }
  } else {
    if(offset < 0) {
      formatString = String("%FT%T-") + String(abs(offset)) + String(":00");
    } else {
      formatString = String("%FT%T+") + String(offset) + String(":00");
    }
  }
  struct tm * timer = gmtime(&timeNow);
  char buf[32];
  strftime(buf, sizeof(buf) - 1, formatString.c_str(), timer);
  return String(buf);
}
int getRawOffset() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  http.begin(client, GEOLOCATION_LINK);
  int code = http.GET();
  JSONVar time;
  time = JSON.parse(http.getString());
  if(code > 0) {
    if(time.hasOwnProperty("raw_offset")) {
      if(time.hasOwnProperty("dst") && time["dst"]) {
        return (int) time["raw_offset"] + 3600;
      } else {
        return time["raw_offset"];
      }
    }
  }
  return -1;
}

void refreshTime() {
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
