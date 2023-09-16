String getToken() {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  http.begin(client, AUTHENTICATION_LINK);
  int code = http.GET();
  if(code > 0) {
    return http.getString();
  } else {
    return "";
  }
}
String refresh(String &refresh_token) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  http.begin(client, REFRESH_LINK);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int code = http.POST("" + refresh_token);
  if(code > 0) {
    JSONVar token = JSON.parse(http.getString());
    if(token.hasOwnProperty("access_token")) {
      return String(token["access_token"]);
    }
  }
  return "";
}
