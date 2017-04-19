// This code is basic P controll.
// Use Timer3 in timer compare interrupt mode and external interrupt for count pulse from encoder.
// Change get command form rs485 function
// init on board led pin
#define LED 8
String versionNum = "03";

byte ID           = 2;                                // arduino ID

//rs 485 variable
const byte EN = 4;
#define BUF_SIZE 32
char buf[ BUF_SIZE ];
uint8_t buf_index = 0;

// init encoder Pin
#define enPinA  2
#define enPinB  3

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

int isConnect = 0;
//setting for fequency 20kHz
#define F_CPU 16000000UL

#define PWM_PERIOD  (1600)  // x 50 usec
/* freq = 20kHz, period = 50 usec
 * 50 usec / 800 = 0.0625 ns -> 16MHz tick frequency
 */
volatile uint16_t ovf_count = 0;

ISR(TIMER1_OVF_vect) { // overflow
  ovf_count++;
}

// init timer1 for adjust PWM Fequncey
void timer1_init() {
  uint8_t SREG_tmp = SREG;  // save the status register
  //   cli();                    // disable interrupts

  TCCR1B = (0 << ICNC1) | (0 << ICES1)
           | (0 << WGM13) | (0 << WGM12)
           | (0 << CS12)  | (0 << CS11) | (0 << CS10);

  TCCR1A = (1 << COM1A1) | (0 << COM1A0)
           | (0 << COM1B1) | (0 << COM1B0)
           | (1 << WGM11)  | (0 << WGM10);

  TCNT1 = 0;
  ICR1  = PWM_PERIOD - 1;
  OCR1A = 0;
  OCR1B = 0;

  TIMSK1 = (0 << ICIE1) | (0 << OCIE1B) | (0 << OCIE1A) | (1 << TOIE1);
  TCCR1B = (0 << ICNC1) | (0 << ICES1)
           | (1 << WGM13) | (1 << WGM12)
           | (0 << CS12)  | (0 << CS11) | (1 << CS10);
  ovf_count = 0;            // reset overflow counter
  SREG = SREG_tmp;          // restore the status register
}

// initialize timer3
void timer3_init() {
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;

  OCR3A = 625;              // compare match register 16MHz/256*(10ms)
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt
}

// active every 10 ms to active rencoder interrupt
ISR(TIMER3_COMPA_vect) {
  if (enable == 0) {
    detachInterrupt(0);
    enable = 1;
    static long countAnt = 0;                     // last count
    speedEn = ((countAnt - countEncoder) * 100 * (60)) / 500; // (encoder pulses X 100 (100 samplimg per sec) X 60 (1 min))/(360 (ppr))
    //    Serial.println(speedEn);
    //    Serial.print("\t");
    // moSpeedNew set-point from cmd
    //    PWM_val = updatePid(PWM_val, moSpeedNew, speedEn);
    // moSpeedNow set-point from inscreae speed
    PWM_val = updatePid(PWM_val, moSpeedNow, speedEn);
    OCR1A = int(map(PWM_val, 0, 6400, 0, PWM_PERIOD));
    countAnt = countEncoder;
    //    Serial.print(moSpeedNew);
    //    Serial.print("\t"); Serial.println(speedEn);
  } else {
    attachInterrupt(0, rencoder, RISING);
    enable = 0;
  }
}

void rencoder()                                      // pulse and direction, direct port reading to save cycles
{
  if (enPinB == LOW)    countEncoder++;                // if(digitalRead(encodPinB1)==HIGH)   count ++;
  else                  countEncoder--;   // if (digitalRead(encodPinB1)==LOW)   count --;
}

int updatePid(int command, int targetValue, int currentValue)                // compute PWM value
{
  float pidTerm = 0;                                                            // PID correction
  int error = 0;
  static int last_error = 0;
  error = abs(targetValue) - abs(currentValue);
  integral += error;
  integral = integral + (error * (10 / 1000));
  if (integral > 1)
  {
    integral = 0.25;
  }
  pidTerm = (Kp * error) + (Kd * (error - last_error) + (Ki * integral ));
  last_error = error;
  return constrain(command + int(pidTerm), 0, 6400);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(enPinA, INPUT);
  pinMode(enPinB, INPUT);

  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(PWM, OUTPUT);

  //  digitalWrite(INA, HIGH);
  //  digitalWrite(INB, LOW);

  // init serial1 for rs 485
  pinMode(EN, OUTPUT);
  digitalWrite(EN, LOW);
  Serial1.begin(38400);
  Serial1.setTimeout(10);

  Serial.begin(115200);
  noInterrupts();
  timer3_init();
  timer1_init();
  interrupts();
  //  cmd = CMD_NONE;
}

void loop() {
  // put your main code here, to run repeatedly:
  getCMD();
  if (millis() - countTime > 10) {
    countTime = millis();
    if (moSpeedNew == 0) {
      if (moSpeedNow > 0) {
        moSpeedNow -= moSpeedStep;
      } else {
        digitalWrite(INA, LOW);
        digitalWrite(INB, LOW);
        is_change_dir = false;
      }
    }
    else if (moSpeedNew > 0) {
      if (!is_change_dir) {
        if (moSpeedNow > 0) {
          moSpeedNow -= moSpeedStep;
        } else {
          digitalWrite(INA, LOW);
          digitalWrite(INB, HIGH);
          is_change_dir = true;
        }
      } else {
        if (moSpeedNow < moSpeedNew) {
          moSpeedNow += moSpeedStep;
        } else {
          moSpeedNow -= moSpeedStep;
        }
      }
    }
    else if (moSpeedNew < 0) {
      if (!is_change_dir) {
        if (moSpeedNow > 0) {
          moSpeedNow -= moSpeedStep;
        } else {
          digitalWrite(INA, HIGH);
          digitalWrite(INB, LOW);
          is_change_dir = true;
        }
      } else {
        if (moSpeedNow < -moSpeedNew) {
          moSpeedNow += moSpeedStep;
        } else {
          moSpeedNow -= moSpeedStep;
        }
      }
    }
  }

  if (millis() - cmdTime > 1000) {
    moSpeedNew = 0;
  }
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
//      sprintf(ack, "%d0,OK\r", ID);
sprintf(ack, "%d0,%d,OK\r", ID, speedEn); 
    }
    digitalWrite(EN, HIGH);
    Serial1.print(ack);
    Serial1.flush();
    delayMicroseconds(200);
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

