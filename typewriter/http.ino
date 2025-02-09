String getCommandResult(String command) {
  HTTPClient http;
  String payload;

  String url = String(api) + "/command/" + command;
  http.begin(url.c_str());
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[HTTP] GET code: %d\n", httpCode);

    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
    }
  } else {
    Serial.printf("[HTTP] GET failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  return payload;
}
