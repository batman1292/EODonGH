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

  Serial.setTimeout( 10 );
}

void loop() {
  // put your main code here, to run repeatedly:
  byte len;
  // read data bytes from Serial and write them to Serial1
  len = Serial.readBytes( (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    digitalWrite( EN, HIGH );
//    Serial1.flush();
    Serial1.write( buf, len );
    Serial1.flush(); // wait for the transmission of outgoing data to complete.
    delayMicroseconds(200); // depending on the baud rate selected for Serial1
    digitalWrite( EN, LOW );
  }
  // read data bytes from Serial1 and write them to Serial
  len = Serial1.readBytes( (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    Serial.flush(); // wait for the transmission of outgoing data to complete.
    Serial.write( buf, len );
  }
}
