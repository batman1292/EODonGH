// init on board led pin
#define LED 8
String versionNum = "02";

byte ID           = 3;                                // arduino ID

//rs 485 variable
const byte EN = 4;
#define BUF_SIZE 32
char buf[ BUF_SIZE ];
uint8_t buf_index = 0;

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
  getCMD();
}


void getCMD() {
  if ( Serial1.available() > 0 ) {
    char ch = Serial1.read();
    if ( (buf_index < BUF_SIZE) && (ch != '\r') ) {
      buf[ buf_index++ ] = ch;
    }
    if ( ch == '\r' ) {
      buf[buf_index] = '\0';
      checkData();
      buf_index = 0;
    }
  }
}

void checkData() {
  String cmd = "";
  char ack[BUF_SIZE];
  if (buf[1] - 48 == ID && buf[buf_index] == '\0') {
    cmd += buf[3];
    cmd += buf[4];
    if (cmd == "LS") {
      buf[6] - 48 == 0 ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      sprintf(ack, "%d0,OK\r", ID);
    }else if(cmd == "M1"){
      cmdTime = millis();
//      if (atoi(&buf[6]) != moSpeedNew) {
        moSpeedNew = atoi(&buf[6]);
        if (moSpeedNew > 5000) {
          moSpeedNew = 5000;
        }
        if (moSpeedNew < -5000) {
          moSpeedNew = -5000;
        }
//      }
      changeDir(moSpeedNew);
      sprintf(ack, "%d0,%d,OK\r", ID, speedEn);
    }
    digitalWrite(EN, HIGH);
    Serial1.print(ack);
    Serial1.flush();
//    delayMicroseconds(200);
    digitalWrite(EN, LOW);
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
