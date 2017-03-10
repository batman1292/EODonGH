#define ledPin 9
#define enPinA  2                               // encoder A pin
#define enPinB  3                               // encoder B pin
                             // actual speed (current speed)
uint8_t enable = 0;
int countLoop = 0;
volatile long count = 0;
int speed_act = 0; 

//debug pin
#define outerProg 5
#define innerProg 7

void setup()
{
  pinMode(outerProg, OUTPUT);
  digitalWrite(outerProg, LOW);
  pinMode(innerProg, OUTPUT);
  digitalWrite(innerProg, LOW);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  //init encoder pin
  pinMode(enPinA, INPUT); 
//  digitalWrite(enPinA, HIGH);       // turn on pull-up resistor
  pinMode(enPinB, INPUT); 
//  digitalWrite(enPinB, HIGH);       // turn on pull-up resistor // encoder pin on interrupt 0 - pin 2
  
  Serial.begin(115200);
  
  noInterrupts();           // disable all interrupts
  
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
  // set compare match register for 1khz increments
  OCR0A = 250;// = (16*10^6) / (2000*64) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |=  (1 << CS01) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  interrupts();             // enable all interrupts
}

ISR(TIMER3_COMPA_vect) {
  static long countAnt = 0;                     // last count
//      speed_act = ((countAnt - count) * (60)) / 5; // (encoder pulses X 100 (100 samplimg per sec) X 60 (1 min))/(360 (ppr))
  Serial.println((countAnt - count)*10*60/500.0);
  countAnt = count;
  attachInterrupt(0, rencoder, RISING);
  enable = 1;
  countLoop = 0;
}

// count to 10 ms and disable rencodeer interrupt
ISR(TIMER0_COMPA_vect) { 
//  digitalWrite(ledPin, digitalRead(ledPin)^1);
  if(enable == 1){
    if(countLoop < 5){
      countLoop++;
    }else{
      detachInterrupt(0);
      enable = 0;
//      static long countAnt = 0;                     // last count
////      speed_act = ((countAnt - count) * (60)) / 5; // (encoder pulses X 100 (100 samplimg per sec) X 60 (1 min))/(360 (ppr))
//      Serial.println((countAnt - count)/0.01);
//      countAnt = count;
    }
  }
}

void rencoder()                                      // pulse and direction, direct port reading to save cycles
{
  if (enPinB == LOW)    count++;                // if(digitalRead(encodPinB1)==HIGH)   count ++;
  else                  count--;   // if (digitalRead(encodPinB1)==LOW)   count --;
}

void loop()
{
  // your program here...
}
