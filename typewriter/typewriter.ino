#define ROW_SELECT_PIN 4

#define CHAR_REPEAT 2
#define TIME_BETWEEN_CHAR 40

String keyMapping[8][9] = {
    {"4", "5", "8", "1", "9", "ß", "FILE", "TW/WP", "`"},
    {"3", "6", "7", "2", "j", "0", "ö", "CANCEL", "l"},
    {"w", "r", "z", "q", "i", "p", "+", "ENTER", "#"},
    {"e", "t", "h", "y", "u", "o", "ü", "IND REV", "k"},
    {"d", "c", "g", "a", "b", "m", "LEFT", "DEL", "."},
    {"x", "f", "v", "s", "n", ",", "DOWN", "WORD OUT", "-"},
    {"", "", "", "", "", "", "CODE", "SPACE", "ä"},
    {"P IND", "UP", "", "LCAPS", "RIGHT", "MENU", "", "SHIFT", ""}};

int pinMapping[8] = { 23, 22, 21, 19, 18, 5, 17, 16 };

long startOfCycle = 0;
String cycleKey = "";
bool newCycle = false;

String lastKey = "";
long startOfCurrentKey = 0;
String currentKey = "";

void rowSelectISR() {
  if (micros() - startOfCycle > 10000)
  {
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

String getKey() {
  long cycleOffset = micros() - startOfCycle;
  for (int t = 0; t < 9; t++)
  {
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

void onKey(String key) {
  Serial.println(key);
  if (key == "SPACE")
  {
    write("helloworld");
  }
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

std::pair<int, int> getPinAndCycleFromKey(String key)
{
  for (int t = 0; t < 9; t++)
  {
    for (int p = 0; p < 8; p++)
    {
      if (keyMapping[p][t] == key)
      {
        return {p, t};
      }
    }
  }
}

void waitForStartOfCycle()
{
  newCycle = false;
  while (!newCycle)
  {
    newCycle = false;
    delayMicroseconds(1);
  }
}

void writeKey(String key)
{
  int charCounter = 0;
  std::pair<int, int> keyInfo = getPinAndCycleFromKey(key);
  int pin = pinMapping[keyInfo.first];
  int timeOffset = keyInfo.second;

  waitForStartOfCycle();

  delay(timeOffset * 2);
  for (int i = 0; i < CHAR_REPEAT; i++)
  {
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

void write(String text)
{
  delay(200);
  for (int i = 0; i < text.length(); i++)
  {
    writeKey(text.substring(i, i + 1));
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
