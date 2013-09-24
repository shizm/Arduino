// Example 03C: ボタンを押すとLEDが点灯し、
// ボタンから指を離した後も点いたままにする
// バウンシングを解消する簡単な方法を取り入れた改良版

const int LED = 13;
const int BUTTON = 7;

int val = 0;
int old_val = 0;
int state = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  val = digitalRead(BUTTON);
  
  if ((val == HIGH) && (old_val == LOW)) {
    state = 1 - state;
    delay(10);
  }
  old_val = val;
  if (state == 1) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
