// This code is basic P controll.
// Use Timer3 in timer compare interrupt mode and external interrupt for count pulse from encoder.
//
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
float Kp          = 5;                                 // PID integral control Gain
float Kd          = 0;                                // PID Derivitave control gain
float Ki          = 0.5;                                // PID Derivitave control gain

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
//    Serial.print(speedEn); Serial.print("\t");
    PWM_val = updatePid(PWM_val, speedRef, speedEn);
    OCR1A = int(map(PWM_val, 0, 6400, 0, PWM_PERIOD));
    countAnt = countEncoder;
    Serial.print(speedEn); Serial.print("\t");Serial.println(PWM_val);
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
  
  digitalWrite(INA, HIGH);
  digitalWrite(INB, LOW);
  
  Serial.begin(115200);
  noInterrupts();
  timer3_init();
  timer1_init();
  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
//  Serial.println("start");
}
