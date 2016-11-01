// ardu485slave module testing code

//Pin description
//pin       <-->  Arduino Pro micro 5V
//tx485     <-->  TX0
//rx485     <-->  RX1
//EN485     <-->  4
//INAdrive  <-->  6
//INBdrive  <-->  5
//PWMdrive  <-->  9
//QEA(En)   <-->  3
//QEB(En)   <-->  2
//INDEX(En) <-->  7
//LED       <-->  8
//Analog    <-->  A0

#define LED 8

//rs 485 variable
#define EN 4
#define BUF_SIZE 32

//smile EVO24V50 variable
#define INA 6
#define INB 5
#define PWN 9

//Encoder Pin
#define QEA 3
#define QEB 2
#define INDEX 7

byte ID = 1;          //change for other pro micro
byte buf[ BUF_SIZE ];
//const byte DIR = 4;

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(EN, LOW);
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
  if (buf[0] - '0' == ID && buf[len - 1] == ':') {
    // fix byte for data
    if (buf[1] == 'L') {
      buf[2]-'0' == 0 ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      digitalWrite(EN, HIGH);
      Serial1.print("ok");
      Serial1.write(buf, len);
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }
  }
}
