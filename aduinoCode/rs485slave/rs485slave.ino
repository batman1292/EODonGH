// test RS 485 Slave on/off pin 17 onboard LED Arduino Pro micro

// ET-MINI RS422/485 Modele (RS485, half-duplex settings)
// ET-MINI  <-->  ProMicro 5V
// VCC      <-->  VCC
// DIR      <-->  D10
// TX       <-->  TX0
// RX       <-->  RX1
// GND      <-->  GND
#define LED 8
#define BUF_SIZE 32
//#define DIR 4

byte ID = 1;          //change for other pro micro
byte buf[ BUF_SIZE ];
const byte DIR = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(DIR, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(DIR, LOW);
  digitalWrite(LED, LOW);
  Serial1.begin(38400);
  Serial1.setTimeout(10);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte len;
  len = Serial1.readBytes( (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    check_data(len);
  }
}

void check_data(byte len) {
  if (buf[0] - '0' == ID && buf[len - 1] == '\n') {
    // fix byte for data
    if (buf[1] == 'L') {
      buf[2]-'0' == 0 ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      digitalWrite(DIR, HIGH);
      Serial1.print("ok");
      Serial1.write(buf, len);
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(DIR, LOW);
    }
    // polling and read data
    //    for(int i=1; i<len; i++){
    //      if(buf[i] != ':'){
    //        if(buf[i]-'0' < 2)
    //          ((buf[i]-'0') == 0) ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
    //      }else{
    //        digitalWrite(DIR, HIGH);
    //        Serial1.print("ok");
    //        Serial1.write(buf, len);
    //        Serial1.flush();
    //        delayMicroseconds(200);
    //        digitalWrite(DIR, LOW);
    //      }
    //    }
  }
}
