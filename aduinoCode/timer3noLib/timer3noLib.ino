#define outerProg 6
#define innerProg 7
int countLoop = 0;

void setup(){
  pinMode(outerProg, OUTPUT);
  digitalWrite(outerProg, LOW);
  pinMode(innerProg, OUTPUT);
  digitalWrite(innerProg, LOW);
//  noInterrupts();           // disable all interrupts
  cli();
  // initialize timer3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  
  OCR3A = 6250;            // compare match register 16MHz/256/10Hz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler 
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt

  //set timer0 interrupt at 2ms
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 50kHz increments
  OCR0A = 40;// = (16*10^6) / 50000 / 8 (prescaler) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 8 prescaler
  TCCR0B |=  (1 << CS01);
  // enable timer compare interrupt
  TIMSK0 |= (0 << OCIE0A);
  sei();             // enable all interrupts
}

ISR(TIMER3_COMPA_vect) {
  digitalWrite(outerProg, HIGH);
  countLoop = 0;
 cli();
  // initialize timer3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  
  OCR3A = 6250;            // compare match register 16MHz/256/10Hz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler 
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt

  //set timer0 interrupt at 2ms
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 50kHz increments
  OCR0A = 40;// = (16*10^6) / 50000 / 8 (prescaler) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 8 prescaler
  TCCR0B |=  (1 << CS01);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  sei();             // enable all interrupts
}

ISR(TIMER0_COMPA_vect) { 
  digitalWrite(innerProg, digitalRead(innerProg) ^ 1);
  if(countLoop < 250){
//    digitalWrite(innerProg, digitalRead(innerProg) ^ 1);
    countLoop++;
  }else{
    digitalWrite(outerProg, LOW);
 cli();
  // initialize timer3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  
  OCR3A = 6250;            // compare match register 16MHz/256/10Hz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler 
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt

  //set timer0 interrupt at 2ms
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 50kHz increments
  OCR0A = 40;// = (16*10^6) / 50000 / 8 (prescaler) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 8 prescaler
  TCCR0B |=  (1 << CS01);
  // enable timer compare interrupt
  TIMSK0 |= (0 << OCIE0A);
  sei();             // enable all interrupts
  }
//  digitalWrite(innerProg, digitalRead(innerProg) ^ 1);
}

void loop(){

}

