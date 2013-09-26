// Example 03A: ボタンを押すとLEDが点灯し、
// ボタンから指を離した後も点いたままにする

const int LED = 13;
const int BUTTON = 7;

int val = 0;
int state = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop() {
  val = digitalRead(BUTTON);
  
  if (val == HIGH) {
    state = 1 - state;
  }
  
  if (state == 1) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
