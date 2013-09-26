// Example 07: アナログ入力ピン0の値をコンピュータに送る
// アップロード後に「Serial Monitor」ボタンを押すこと

const int SENSOR = 0;
int val = 0;

void setup() {
  Serial.begin(9800);
}

void loop() {
  val = analogRead(SENSOR);
  Serial.println(val);
  delay(1000);
}
