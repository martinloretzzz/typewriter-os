void waitForStartOfCycle() {
  newCycle = false;
  while (!newCycle) {
    newCycle = false;
    delayMicroseconds(1);
  }
}

KeyInfo getPinAndCycleFromKey(String key) {
  for (int t = 0; t < 9; t++) {
    for (int p = 0; p < 8; p++) {
      if (keyMapping[p][t] == key) {
        return {false, p, t};
      }
      if (keyMappingShift[p][t] == key) {
        return {true, p, t};
      }
    }
  }
  return {false, 5, 0};
}

void writeKey(String key) {
  int charCounter = 0;
  KeyInfo keyInfo = getPinAndCycleFromKey(key);
  int pin = pinMapping[keyInfo.pin];
  int timeOffset = keyInfo.offset;

  bool shift = keyInfo.shift;
  int shiftPin = pinMapping[7];
  int shiftOffset = 7;

  waitForStartOfCycle();

  for (int i = 0; i < 3; i++) {
    for (int t = 0; t < 9; t++) {
      delay(1);
      // Shift key needs to be pressed before key itself
      if (!shift || (shift && i != 0)) {
        if (t == timeOffset) {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, LOW);
        }
        if (t == timeOffset + 1) {
          pinMode(pin, INPUT);
        }
      }

      if (shift && t == shiftOffset) {
        pinMode(shiftPin, OUTPUT);
        digitalWrite(shiftPin, LOW);
      }
      if (shift && t == shiftOffset + 1) {
        pinMode(shiftPin, INPUT);
      }
      delay(1);
    }
    pinMode(pin, INPUT);
  }

  delay(TIME_BETWEEN_CHAR);
}

void write(String text) {
  delay(200);
  for (int i = 0; i < text.length(); i++) {
    writeKey(text.substring(i, i + 1));
  }
}