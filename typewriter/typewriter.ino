#define ROW_SELECT_PIN 4

long startOfCycle = 0;

void rowSelectISR()
{
  startOfCycle = micros();
}

void setup()
{
  Serial.begin(115200);
  pinMode(23, INPUT);
  pinMode(22, INPUT);
  pinMode(21, INPUT);
  pinMode(19, INPUT);
  pinMode(18, INPUT);
  pinMode(5, INPUT);
  pinMode(17, INPUT);
  pinMode(16, INPUT);
  pinMode(ROW_SELECT_PIN, INPUT);

  attachInterrupt(ROW_SELECT_PIN, rowSelectISR, RISING);
}

void printPinActive(int pin)
{
  long cycleOffset = ((micros() - (startOfCycle - 2000)) % 18000);

  for (int i = 0; i < 9; i++)
  {
    if (digitalRead(pin) == LOW && 2000 * i + 500 < cycleOffset && cycleOffset < 2000 * (i + 1) - 500)
    {
      Serial.print(pin);
      Serial.print(" ");
      Serial.print(i);
      Serial.print(" ");
      Serial.print(cycleOffset);
      Serial.println("");
    }
  }
}

void loop()
{
  printPinActive(23);
  printPinActive(22);
  printPinActive(21);
  printPinActive(19);
  printPinActive(18);
  printPinActive(5);
  printPinActive(17);
  printPinActive(16);
  delayMicroseconds(500);
}
