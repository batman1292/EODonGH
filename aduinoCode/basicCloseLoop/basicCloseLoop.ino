// ardu485slave basic close loop control testing code

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

//smile EVO24V50 variable
#define INA 6
#define INB 5
const uint8_t PWM = 9;

// encoder Pin
#define enPinA  2
#define enPinB  3

uint8_t enable = 0;
int countLoop = 0;
int countUpdate = 0;
volatile long count = 0;
int speed_act = 0;
int speed_req = 1500;
int PWM_val = 0;
int integral = 0;
float Kp =   2;                                 // PID integral control Gain
float Kd =    0;                                // PID Derivitave control gain
float Ki =    0.5;                                // PID Derivitave control gain

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

// active every 20 ms to active rencoder interrupt
ISR(TIMER3_COMPA_vect) {
  static long countAnt = 0;                     // last count
  speed_act = ((countAnt - count) * 100 * (60)) / 500; // (encoder pulses X 100 (100 samplimg per sec) X 60 (1 min))/(360 (ppr))
  Serial.println(speed_act);
//  Serial.print("\t");
  PWM_val = updatePid(PWM_val, speed_req, speed_act);
//  Serial.println(PWM_val);
  OCR1A = int(map(PWM_val, 0, 255, 0, PWM_PERIOD));
  countAnt = count;
//  countUpdate++;
//  if (countUpdate == 5) {
    attachInterrupt(0, rencoder, RISING);
    enable = 1;
    countLoop = 0;
//    countUpdate = 0;
//  }
}

void timer3_init() {
  // initialize timer3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;

  OCR3A = 1250;            // compare match register 16MHz/256/10Hz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt
}

// count to 10 ms and disable rencoder interrupt
ISR(TIMER0_COMPA_vect) {
  if (enable == 1) {
    if (countLoop < 5) {
      countLoop++;
    } else {
      detachInterrupt(0);
      enable = 0;
      //      static long countAnt = 0;
      //      speed_act = ((countAnt - count) * 10 * (60)) / 500; // (encoder pulses X 100 (100 samplimg per sec) X 60 (1 min))/(360 (ppr))
      //      Serial.print(speed_act); Serial.print("\t");
      //      PWM_val = updatePid(PWM_val, speed_req, speed_act);
      //      Serial.println(PWM_val);
      //      OCR1A = int(map(PWM_val, 0, 255, 0, PWM_PERIOD));
      //      countAnt = count;
    }
  }
}

void timer0_init() {
  //set timer0 interrupt at 2ms
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 1khz increments
  OCR0A = 250;// = (16*10^6) / (2000*64) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |=  (1 << CS01) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
}

void rencoder()                                      // pulse and direction, direct port reading to save cycles
{
  if (enPinB == LOW)    count++;                // if(digitalRead(encodPinB1)==HIGH)   count ++;
  else                  count--;   // if (digitalRead(encodPinB1)==LOW)   count --;
}

void setup() {
  // put your setup code here, to run once:
  pinMode(INA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(PWM, OUTPUT);

  pinMode(enPinA, INPUT);
  pinMode(enPinB, INPUT);
  Serial.begin(115200);
  noInterrupts();
  timer3_init();
  timer0_init();
  timer1_init();
  interrupts();
  digitalWrite(INA, HIGH);
  digitalWrite(INB, LOW);
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
  return constrain(command + int(pidTerm), 0, 255);
}

void loop() {
  // put your main code here, to run repeatedly:

}
