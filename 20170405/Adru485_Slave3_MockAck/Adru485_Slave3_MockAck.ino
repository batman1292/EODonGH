// init on board led pin
#define LED 8
String versionNum = "02";

byte ID           = 3;                                // arduino ID
String cmd        = "";

//rs 485 variable
const byte EN = 4;
#define BUF_SIZE 32
byte buf[ BUF_SIZE ];

// init drive motor pin variable
#define INA 6
#define INB 5
const uint8_t PWM = 9;

// init golbal variable
int countEncoder  = 0;      // count pulse from encoder
uint8_t enable    = 1;      // enable external interrupt for read encoder
int speedEn       = 0;      // speed from encoder
int speedRef      = 2000;   // speed value for control
int PWM_val       = 0;
int integral      = 0;
// value
float Kp          = 2;                                // PID integral control Gain
float Kd          = 0;                                // PID Derivitave control gain
float Ki          = 0;                              // PID Derivitave control gain

int moSpeedNew = 0;
int moSpeedNow = 0;
int moSpeedStep = 100;
bool is_change_dir = false;
byte state_dir = 0;
long countTime = millis();
long cmdTime = millis();

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
  len = Serial1.readBytesUntil('\r', (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    check_data(len);
  }
   if (millis() - cmdTime > 1000) {
//    Serial.println("timeout");
    moSpeedNew = 0;
  }
}

void check_data(byte len) {
#ifdef DEBUG
  Serial.print(len);
  Serial.write(buf, len);
  Serial.print("\n");
#endif
  if (buf[1] - '0' == ID) {
    //    digitalWrite(LED, HIGH);
    cmd += char(buf[3]);
    cmd += char(buf[4]);
    if (cmd == "LS") {
      buf[6] - '0' == 0 ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      digitalWrite(EN, HIGH);
      Serial1.print(ID);
      Serial1.print("0");
      Serial1.print(",OK\r");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "VR") {
      digitalWrite(EN, HIGH);
      Serial1.print(ID);
      Serial1.print("0");
      Serial1.print(",OK,");
      Serial1.print(versionNum);
      Serial1.print("\n");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
      //    }
    } else if (cmd == "M1") {
      cmdTime = millis();
      char temp[6];
      int i;
      for (i = 6; i < len; i++) {
        temp[i - 6] = char(buf[i]);
      }
      temp[i - 6] = '\0';
      if (atoi(temp) != moSpeedNew) {
        moSpeedNew = atoi(temp);
        if (moSpeedNew > 5000) {
          moSpeedNew = 5000;
        }
        if (moSpeedNew < -5000) {
          moSpeedNew = -5000;
        }
      }
      //      Serial.println(moSpeedNew);
      changeDir(moSpeedNew);
      digitalWrite(EN, HIGH);
//      Serial1.print("0");
      Serial1.print(ID);
//      Serial1.print(',');
      Serial1.print("0");
//      Serial1.print(speedEn);
      Serial1.print(",OK\r");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }
    cmd = "";
    //    digitalWrite(LED, LOW);
  }
}

void changeDir(int moSpeed) {
  if (moSpeed > 0) {
    if (state_dir != 1) {
      is_change_dir = false;
      state_dir = 1;
    }
  } else if (moSpeed < 0) {
    if (state_dir != 2) {
      is_change_dir = false;
      state_dir = 2;
    }
  }
}
