byte buf[32];
const byte EN = 4;
#define BUF_SIZE 32

void setup() {
  // put your setup code here, to run once:
  // init serial1 for rs 485
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  Serial1.begin(38400);
  Serial1.setTimeout(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte len;
  len = Serial1.readBytes( (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    digitalWrite(EN, HIGH);
    Serial1.write(buf, len);
    Serial1.flush();
    delayMicroseconds(200);
    digitalWrite(EN, LOW);
  }
}
