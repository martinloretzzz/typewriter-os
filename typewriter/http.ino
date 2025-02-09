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
  return removeStringQuotes(payload);
}

String removeStringQuotes(String input) {
  String text = input;
  if (input.length() > 1 && input.charAt(0) == '"' && input.charAt(input.length() - 1) == '"') {
    text = input.substring(1, input.length() - 1);
  }
  text.replace("\\n", "\n");
  return text;
}