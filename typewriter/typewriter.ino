#define ROW_SELECT_PIN 4

#define CHAR_REPEAT 2
#define TIME_BETWEEN_CHAR 40

String keyMapping[8][9] = {
  { "4", "5", "8", "1", "9", "ß", "FILE", "TW/WP", "`" },
  { "3", "6", "7", "2", "j", "0", "ö", "CANCEL", "l" },
  { "w", "r", "z", "q", "i", "p", "+", "ENTER", "#" },
  { "e", "t", "h", "y", "u", "o", "ü", "IND REV", "k" },
  { "d", "c", "g", "a", "b", "m", "LEFT", "DEL", "." },
  { "x", "f", "v", "s", "n", ",", "DOWN", "WORD OUT", "-" },
  { "", "", "", "", "", "", "CODE", "SPACE", "ä" },
  { "P IND", "UP", "", "LCAPS", "RIGHT", "MENU", "", "SHIFT", "" }
};

int pinMapping[8] = { 23, 22, 21, 19, 18, 5, 17, 16 };

long startOfCycle = 0;
String cycleKey = "";
bool newCycle = false;

String lastKey = "";
long startOfCurrentKey = 0;
String currentKey = "";

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
}

void onKey(String key) {
  Serial.println(key);
  if (key == "SPACE") {
    write("helloworld");
  }
}

void loop() {
  updateRead();
  delayMicroseconds(500);
}
