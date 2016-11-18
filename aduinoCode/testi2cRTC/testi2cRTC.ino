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
#include <Wire.h>
#include <RtcDS3231.h>

RtcDS3231 rtc;

#define LED 8

//rs 485 variable
#define EN 4
#define BUF_SIZE 32

//smile EVO24V50 variable
#define INA 6
#define INB 5
#define PWM 9

//Encoder Pin
#define QEA 3
#define QEB 2
#define INDEX 7

const int AN = A0;
byte ID = 3;          //change for other pro micro
byte buf[ BUF_SIZE ];
String cmd = "";
//long countTime = millis();
int analogValue = 0;
byte isConnect = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  // init serial1 for rs 485
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  Serial1.begin(38400);
  Serial1.setTimeout(10);
  //  init evo24v50 pin
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(PWM, OUTPUT);
  //  debug serial
  Serial.begin(115200);
  rtc.Begin();
  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  rtc.Enable32kHzPin(false);
  rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte len;
  len = Serial1.readBytesUntil('\r', (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    check_data(len);
    //    Serial.write(buf, len);
  }
}

void check_data(byte len) {
  cmd += char(buf[0]);
  cmd += char(buf[1]);
  if (cmd.toInt()) {
    cmd = "";
    cmd += char(buf[3]);
    cmd += char(buf[4]);
    if (cmd == "CT") {
      isConnect = 1;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "DT") {
      isConnect = 0;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "LS") {
      buf[6] - '0' == 0 ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "AN") {
      analogValue = analogRead(AN);
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",AN,");
      Serial1.println(analogValue);
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "TM") {
      RtcDateTime now = rtc.GetDateTime();
      char date [BUF_SIZE];
      sprintf(date, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",OK,");
      Serial1.println(date);
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }
  }
  cmd = "";
}
