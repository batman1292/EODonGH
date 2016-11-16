#include <string.h>
//rs 485 variable
const byte EN = 4;
#define BUF_SIZE 32

byte ID = 2;          //change for other pro micro
byte buf[ BUF_SIZE ];
String cmd = "";
//char *temp, *cmd, *value;

void setup() {
  // put your setup code here, to run once:
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  Serial1.begin(38400);
  Serial1.setTimeout(10);
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte len;
  len = Serial1.readBytesUntil('\r', (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    check_data(len);
  }
}

void check_data(byte len) {
  Serial.write(buf, len);
  Serial.println();
//  i = (char *)buf;
//  while (i != NULL) {
//    p = strtok_r(i, ",", &i);
//    Serial.println(p);
//  }
}

