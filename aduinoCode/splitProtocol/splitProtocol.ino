
//rs 485 variable
const byte EN = 4;
#define BUF_SIZE 32

byte ID = 13;          //change for other pro micro
byte buf[ BUF_SIZE ];
String cmd = "";

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
  len = Serial1.readBytes( (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    check_data(len);
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
      isConnect = 1;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }else if(cmd == "DT"){
      isConnect = 0;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.println(",OK");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }
  }
  cmd = "";
}

