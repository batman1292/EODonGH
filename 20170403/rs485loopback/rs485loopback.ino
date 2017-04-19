//rs 485 variable
const byte EN = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  Serial1.begin(38400);
  Serial1.setTimeout(10);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(EN, HIGH);
  Serial1.println("0");
//  Serial1.print(ID);
  Serial1.flush();
  delayMicroseconds(200);
  digitalWrite(EN, LOW);
  delay(500);
  Serial.println("ddd");
}
