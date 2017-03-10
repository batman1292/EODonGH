#define debugT3 6
#define debugT0 5
// active every 20 ms to active rencoder interrupt
ISR(TIMER3_COMPA_vect) {
  static long countAnt = 0;                     // last count
  speed_act = ((countAnt - count) * 100 * (60)) / 500; // (encoder pulses X 100 (100 samplimg per sec) X 60 (1 min))/(360 (ppr))
  Serial.println(speed_act);
  //  Serial.print("\t");
  PWM_val = updatePid(PWM_val, moSpeedNow, speed_act);
  //  Serial.println(PWM_val);
  OCR1A = int(map(PWM_val, 0, 6400, 0, PWM_PERIOD));
  countAnt = count;
  //  countUpdate++;
  //  if (countUpdate == 5) {
  TIMSK0 |= (1 << OCIE0A);
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

  OCR3A = 6250;            // compare match register 16MHz/256/10Hz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt
}

// count to 5 ms and disable rencoder interrupt
ISR(TIMER0_COMPA_vect) {
  if (enable == 1) {
    if (countLoop < 5) {
      digitalW
      countLoop++;
    } else {
      TIMSK0 = 0;
      detachInterrupt(0);
      enable = 0;
    }
  }
//  Serial.println("disable");
}

void timer0_init() {
  //set timer0 interrupt at 2ms
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 1khz increments
  OCR0A = 250;// = (16*10^6) / (1000*64) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |=  (1 << CS01) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(debugT3, OUTPUT);
  pinMode(debugT0, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
