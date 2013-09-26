// Example 06A: アナログ入力の値に応じてLEDの点滅レートが変化

const int LED = 13;
int val = 0;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  val = analogRead(0);
  
  digitalWrite(LED, HIGH);
  delay(val);
  
  digitalWrite(LED, LOW);
  delay(val);
}
