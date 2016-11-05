#include <Wire.h>
#include <RtcDS3231.h>

RtcDS3231 rtc;

#define LED 8

//rs 485 variable
const byte EN = 4;
#define BUF_SIZE 32

byte ID = 3;
byte buf[BUF_SIZE];
String cmd = "";
const int AN = A0;
int analogValue = 0;

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(38400);
  Serial1.setTimeout(10);
  rtc.Begin();
  // never assume the Rtc was last configured by you, so
  // just clear them to your needed state
  rtc.Enable32kHzPin(false);
  rtc.SetSquareWavePin(DS3231SquareWavePin_ModeNone);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte len;
  len = Serial1.readBytes( (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    check_data(len);
    //    Serial.write(buf, len);
  }
}

void check_data(byte len) {
  cmd += char(buf[0]);
  cmd += char(buf[1]);
  if (cmd.toInt() == ID && buf[len - 1] == '\n') {
    cmd = "";
    cmd += char(buf[3]);
    cmd += char(buf[4]);
    if(cmd == "CT"){
//      isConnect = 1;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }else if(cmd == "DT"){
//      isConnect = 0;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }else if(cmd == "LS"){
      buf[6] - '0' == 0 ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }else if(cmd == "AN"){
      analogValue = analogRead(AN);
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",AN,");
      Serial1.println(analogValue);
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }else if(cmd == "TM"){
      RtcDateTime now = rtc.GetDateTime();
      char date [BUF_SIZE];
      sprintf(date, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",AN,");
      Serial1.println(date);
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }
  }
  cmd = "";
}

