String getRequest(String &access_token, String &time) {
  HTTPClient http;
  WiFiClientSecure client;
  client.setInsecure();
  http.begin(client, String(REQUEST_LINK) + "&timeMin=" + time + "&access_token=" + access_token);
  int code = http.GET();
  if(code == 200) {
    return http.getString();
  } 
  return "";
}
