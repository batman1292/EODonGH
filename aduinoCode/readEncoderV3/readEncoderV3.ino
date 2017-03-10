// This code read data from Encoder and calculate motor speed in RPM.
// Use Timer3 in timer compare interrupt mode and external interrupt for count pulse from encoder.

// init encoder Pin
#define enPinA  2
#define enPinB  3

// init golbal variable
int countEncoder  = 0;      // count pulse from encoder
uint8_t enable    = 1;      // enable external interrupt for read encoder
int speedEn       = 0;      // speed from encoder

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
    Serial.println(speedEn);
//    PWM_val = updatePid(PWM_val, speed_req, speed_act);
    countAnt = countEncoder;
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

void setup() {
  // put your setup code here, to run once:
  pinMode(enPinA, INPUT);
  pinMode(enPinB, INPUT);
  Serial.begin(115200);
  noInterrupts();
  timer3_init();
  interrupts();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("start");
}
