void delayMicrosecondsNonBlocking(unsigned long us) {
  unsigned long start = micros();
  while (micros() - start < us) {
    yield();
  }
}

void delayNonBlocking(unsigned long ms) {
  delayMicrosecondsNonBlocking(1000 * ms);
}

void waitForStartOfCycle() {
  newCycle = false;
  while (!newCycle) {
    newCycle = false;
    delayMicrosecondsNonBlocking(1);
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
  // Space for invalid char
  return {false, 6, 8};
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

  for (int i = 0; i < 4; i++) {
    for (int t = 0; t < 9; t++) {
      delayNonBlocking(1);
      // Shift key needs to be pressed before key itself
      if (!shift || (shift && i != 0)) {
        if (t == timeOffset) {
          pinMode(pin, OUTPUT);
          digitalWrite(pin, LOW);
        }
        if ((t == timeOffset + 1) || (timeOffset == 8 && t == 0)) {
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
      delayNonBlocking(1);
    }
  }
  pinMode(pin, INPUT);

  delayNonBlocking(TIME_BETWEEN_CHAR);
}

void write(String text) {
  delayNonBlocking(200);
  for (int i = 0; i < text.length(); i++) {
    char c = text.charAt(i);
    writeKey(String(c));
    Serial.print(c);
  }
  Serial.println();
}