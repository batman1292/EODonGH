// ardu485slave module drive testing code

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

String versionNum = "01";
#define LED 8
//#define DEBUG
//rs 485 variable
const byte EN = 4;
#define BUF_SIZE 32

//smile EVO24V50 variable
#define INA 6
#define INB 5
const uint8_t PWM = 9;

byte ID = 3; // change for arduino
byte buf[ BUF_SIZE ];
String cmd = "";

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

void timer1_init() {
   uint8_t SREG_tmp = SREG;  // save the status register
   cli();                    // disable interrupts

   TCCR1B = (0 << ICNC1) | (0 << ICES1) 
          | (0 << WGM13) | (0 << WGM12) 
          | (0 << CS12)  | (0 << CS11) | (0 << CS10);

   TCCR1A = (1 << COM1A1) | (0 << COM1A0)
          | (0 << COM1B1) | (0 << COM1B0)
          | (1 << WGM11)  | (0 << WGM10); 

   TCNT1 = 0; 
   ICR1  = PWM_PERIOD-1;
   OCR1A = 0;
   OCR1B = 0;

   TIMSK1 = (0 << ICIE1) | (0 << OCIE1B) | (0 << OCIE1A) | (1 << TOIE1); 
   TCCR1B = (0 << ICNC1) | (0 << ICES1) 
          | (1 << WGM13) | (1 << WGM12)
          | (0 << CS12)  | (0 << CS11) | (1 << CS10);
   ovf_count = 0;            // reset overflow counter
   SREG = SREG_tmp;          // restore the status register
}

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
  timer1_init();
  sei();
//  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte len;
  len = Serial1.readBytesUntil('\r', (char *)buf, BUF_SIZE );
  if ( len > 0 ) {
    check_data(len);
  }
  //  if (isConnect) {
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
//    pwmWrite(PWM, map(moSpeedNow, 0, 6400, 0, 255));
    OCR1A = int(map(moSpeedNow, 0, 6400, 0, PWM_PERIOD));
  }
  if (millis() - cmdTime > 1000) {
    moSpeedNew = 0;
  }
  //  }
}

void check_data(byte len) {
  #ifdef DEBUG
  Serial.print(len);
  Serial.write(buf, len);
  Serial.print("\n");
  #endif
  if (buf[1] - '0' == ID) {
    digitalWrite(LED, HIGH);
    cmd += char(buf[3]);
    cmd += char(buf[4]);
    if (cmd == "LS") {
      buf[6] - '0' == 0 ? digitalWrite(LED, LOW) : digitalWrite(LED, HIGH);
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",OK\r");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "VR") {
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",OK,");
      Serial1.print(versionNum);
      Serial1.print("\r");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "M1") {
      cmdTime = millis();
      String value = "";
      for (int i = 6; i < len; i++) {
        value += char(buf[i]);
      }
      moSpeedNew = value.toInt();
      if (moSpeedNew > 5000) {
        moSpeedNew = 5000;
      }
      if (moSpeedNew < -5000) {
        moSpeedNew = -5000;
      }
      changeDir(moSpeedNew);
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",OK\r");
      //      Serial1.println(moSpeedNow);
      Serial1.flush();
      delayMicroseconds(20);
      digitalWrite(EN, LOW);
    } else if (cmd == "CT") {
      isConnect = 1;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",OK\r");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "DT") {
      isConnect = 0;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",OK\r");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    } else if (cmd == "BK") {
      moSpeedNew = 0;
      digitalWrite(EN, HIGH);
      Serial1.print("0");
      Serial1.print(ID);
      Serial1.print(",OK\r");
      Serial1.flush();
      delayMicroseconds(200);
      digitalWrite(EN, LOW);
    }
    cmd = "";
    digitalWrite(LED, LOW);
  }else{
//    buf = "";
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

