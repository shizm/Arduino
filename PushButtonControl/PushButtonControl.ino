// Example 02: ボタンが押されている間、LEDを点ける

const int LED = 13;
const int BUTTON = 7;

int val = 0;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);
}

void loop()
{
  val = digitalRead(BUTTON);
  
  // 入力はHIGH(ボタンが押されている状態)か？
  if (val == HIGH) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
}
