void waitForStartOfCycle() {
  newCycle = false;
  while (!newCycle) {
    newCycle = false;
    delayMicroseconds(1);
  }
}

std::pair<int, int> getPinAndCycleFromKey(String key) {
  for (int t = 0; t < 9; t++) {
    for (int p = 0; p < 8; p++) {
      if (keyMapping[p][t] == key) {
        return { p, t };
      }
    }
  }
  return {5, 0};
}

void writeKey(String key) {
  int charCounter = 0;
  std::pair<int, int> keyInfo = getPinAndCycleFromKey(key);
  int pin = pinMapping[keyInfo.first];
  int timeOffset = keyInfo.second;

  waitForStartOfCycle();

  delay(timeOffset * 2);
  for (int i = 0; i < CHAR_REPEAT; i++) {
    delay(1);
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    delay(2);
    pinMode(pin, INPUT);
    delay(1);

    delay(14);
  }

  delay(TIME_BETWEEN_CHAR);
}

void write(String text) {
  delay(200);
  for (int i = 0; i < text.length(); i++) {
    writeKey(text.substring(i, i + 1));
  }
}