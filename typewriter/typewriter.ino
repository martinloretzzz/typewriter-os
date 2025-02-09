#include <WiFi.h>
#include <ESPmDNS.h>
#include <NetworkUdp.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>

#include "config.h"
// Set these values in a config.h:
// const char *ssid = "...";
// const char *password = "...";
// const char *api = "...";

#define ROW_SELECT_PIN 4
#define TIME_BETWEEN_CHAR 40

String keyMapping[8][9] = {
  { "4", "5", "8", "1", "9", "ß", "FILE", "TW/WP", "´" },
  { "3", "6", "7", "2", "j", "0", "ö", "CANCEL", "l" },
  { "w", "r", "z", "q", "i", "p", "+", "\n", "#" },
  { "e", "t", "h", "y", "u", "o", "ü", "IND REV", "k" },
  { "d", "c", "g", "a", "b", "m", "LEFT", "DEL", "." },
  { "x", "f", "v", "s", "n", ",", "DOWN", "WORD OUT", "-" },
  { "", "", "", "", "", "", "CODE", " ", "ä" },
  { "P IND", "UP", "", "LCAPS", "RIGHT", "MENU", "", "SHIFT", "" }
};

String keyMappingShift[8][9] = {
  { "$", "%", "(", "!", ")", "ß", "", "", "`" },
  { "§", "&", "/", "\"", "j", "=", "Ö", "", "lL" },
  { "W", "R", "Z", "Q", "I", "P", "*", "", "#'" },
  { "E", "T", "H", "Y", "U", "O", "Ü", "", "K" },
  { "D", "C", "G", "A", "B", "M", "", "", ":" },
  { "X", "F", "V", "S", "N", ",", "", "", "_" },
  { "", "", "", "", "", "", "", " ", "Ä" },
  { "", "", "", "", "", "", "", "", "" }
};

int pinMapping[8] = { 23, 22, 21, 19, 18, 5, 17, 16 };

long startOfCycle = 0;
String cycleKey = "";
bool newCycle = false;

String lastKey = "";
long startOfCurrentKey = 0;
String currentKey = "";

struct KeyInfo {
  bool shift;
  int pin;
  int offset;
};

void rowSelectISR() {
  if (micros() - startOfCycle > 10000) {
    startOfCycle = micros();
    newCycle = true;
  }
}

void setup() {
  Serial.begin(115200);
  for (int p = 0; p < 8; p++) {
    pinMode(pinMapping[p], INPUT);
  }
  pinMode(ROW_SELECT_PIN, INPUT);
  attachInterrupt(ROW_SELECT_PIN, rowSelectISR, FALLING);

  setupWiFi();
  setupOTA();
}

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void onKey(String key) {
  Serial.println(key);
  if (key == " ") {
    write("weather");
    String answer = getCommandResult("weather");
    write(answer);
    Serial.println(answer);
  }
}

void loop() {
  updateRead();
  updateOTA();
  delayMicroseconds(500);
}
