#define ROW_SELECT_PIN 4

String keyMapping[8][9] = {
  { "5", "8", "1", "9", "ß", "FILE", "TW/WP", "`", "4" },
  { "6", "7", "2", "j", "0", "ö", "CANCEL", "l", "3" },
  { "r", "z", "q", "i", "p", "+", "ENTER", "#", "w" },
  { "t", "h", "y", "u", "o", "ü", "IND REV", "k", "e" },
  { "c", "g", "a", "b", "m", "LEFT", "DEL", ".", "d" },
  { "f", "v", "s", "n", ",", "DOWN", "WORD OUT", "-", "x" },
  { "", "", "", "", "", "CODE", "SPACE", "ä", "" },
  { "UP", "", "LCAPS", "RIGHT", "MENU", "", "SHIFT", "", "P IND" }
};

int pinMapping[8] = { 23, 22, 21, 19, 18, 5, 17, 16 };

long startOfCycle = 0;
String cycleKey = "";
bool newCycle = false;

String lastKey = "";
long startOfCurrentKey = 0;
String currentKey = "";

void rowSelectISR() {
  startOfCycle = micros();
  newCycle = true;
}

void setup() {
  Serial.begin(115200);
  for (int p = 0; p < 8; p++) {
    pinMode(pinMapping[p], INPUT);
  }
  pinMode(ROW_SELECT_PIN, INPUT);

  attachInterrupt(ROW_SELECT_PIN, rowSelectISR, RISING);
}

String getKey() {
  long cycleOffset = (micros() - startOfCycle) - 2000;
  for (int t = -1; t < 8; t++) {
    if (2000 * t + 500 < cycleOffset && cycleOffset < 2000 * (t + 1) - 500) {
      for (int p = 0; p < 8; p++) {
        if (digitalRead(pinMapping[p]) == LOW) {
          return keyMapping[p][t + 1];
        }
      }
    }
  }
  return "";
}

void onKey(String key) {
  Serial.println(key);
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

void loop() {
  String pressedKey = getKey();
  if (pressedKey != "") {
    cycleKey = pressedKey;
  }

  if (newCycle) {
    onCycleKey(cycleKey);
    newCycle = false;
    cycleKey = "";
  }

  delayMicroseconds(500);
}
