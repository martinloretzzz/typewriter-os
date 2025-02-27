String getKey() {
  long cycleOffset = micros() - startOfCycle;
  for (int t = 0; t < 10; t++) {
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

String keyToShiftKey(String key) {
  for (int t = 0; t < 9; t++) {
    for (int p = 0; p < 8; p++) {
      if (keyMapping[p][t] == key) {
        return keyMappingShift[p][t];
      }
    }
  }
  return "";
}

void updateRead() {
  String pressedKey = getKey();
  if (pressedKey != "" && pressedKey != "SHIFT") {
    cycleKey = pressedKey;
  }
  if (pressedKey == "SHIFT") {
    shiftPressed = true;
  }
  

  if (newCycle) {
    if (shiftPressed) {
      cycleKey = keyToShiftKey(cycleKey);
    }
    onCycleKey(cycleKey);
    newCycle = false;
    cycleKey = "";
    shiftPressed = false;
  }
}