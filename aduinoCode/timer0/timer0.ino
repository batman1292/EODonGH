boolean toggle0 = 0;
ISR(TIMER0_COMPA_vect) { //timer0 interrupt 2kHz toggles pin 8
  //generates pulse wave of frequency 2kHz/2 = 1kHz (takes two cycles for full wave- toggle high then toggle low)
  //  if (toggle0){
  //    digitalWrite(9,HIGH);
  //    toggle0 = 0;
  //  }
  //  else{
  //    digitalWrite(9,LOW);
  //    toggle0 = 1;
  //  }
  digitalWrite(9, digitalRead(9)^1);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(9, OUTPUT);
  noInterrupts();           // disable all interrupts
  //set timer0 interrupt at 2ms
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 124;// = (16*10^6) / (2000*64) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |=  (1 << CS02);
  // enable timer compare interrupt
  TIMSK0 |= (0 << OCIE0A);
  interrupts();             // enable all interrupts
}

void loop() {
  // put your main code here, to run repeatedly:

}
