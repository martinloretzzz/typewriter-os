String getKey() {
  long cycleOffset = micros() - startOfCycle;
  for (int t = 0; t < 9; t++) {
    if (2000 * t + 500 < cycleOffset && cycleOffset < 2000 * (t + 1) - 500) {
      for (int p = 0; p < 8; p++) {
        if (digitalRead(pinMapping[p]) == LOW) {
          return keyMapping[p][t];
        }
      }
    }
  }
  return "";
}

void onCycleKey(String key) {
  if (key != currentKey && key != lastKey) {
    currentKey = key;
    startOfCurrentKey = millis();
  }

  if (key == currentKey && millis() - startOfCurrentKey > 20) {
    if (key != "") {
      onKey(key);
    }
    lastKey = key;
    currentKey = "";
  }
}

void updateRead() {
  String pressedKey = getKey();
  if (pressedKey != "") {
    cycleKey = pressedKey;
  }

  if (newCycle) {
    onCycleKey(cycleKey);
    newCycle = false;
    cycleKey = "";
  }
}