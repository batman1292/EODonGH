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
//  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
//  byte len;
//  len = Serial1.readBytes( (char *)buf, BUF_SIZE );
//  if ( len > 0 ) {
//    //    check_data(len);
//    Serial.write(buf, len);
//    digitalWrite(EN, HIGH);
////    Serial1.print("0");
//    Serial1.write(buf, len);
////    Serial1.println(",OK");
//    Serial1.flush();
//    delayMicroseconds(200);
//    digitalWrite(EN, LOW);
//  }
  digitalWrite(EN, HIGH);
  delayMicroseconds(50);
  Serial1.write(0x55);
  Serial1.write(0x56);
  Serial1.write(0x57);
//  Serial1.write(0x58);
//  Serial1.write(0x59);
//  Serial1.write(0x60);
  Serial1.flush();
  delayMicroseconds(200);
  digitalWrite(EN, LOW);
  delay(100);
}
