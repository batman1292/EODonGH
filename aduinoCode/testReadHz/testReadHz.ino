#define enPinA  2                               // encoder A pin
#define enPinB  3                               // encoder B pin

uint8_t enable = 0;
volatile long count = 0;

void setup()
{
//  pinMode(outerProg, OUTPUT);
//  digitalWrite(outerProg, LOW);
//  pinMode(innerProg, OUTPUT);
//  digitalWrite(innerProg, LOW);
  
//  pinMode(ledPin, OUTPUT);
//  digitalWrite(ledPin, LOW);
  //init encoder pin
  pinMode(enPinA, INPUT); 
  digitalWrite(enPinA, HIGH);       // turn on pull-up resistor
  pinMode(enPinB, INPUT); 
  digitalWrite(enPinB, HIGH);       // turn on pull-up resistor // encoder pin on interrupt 0 - pin 2
  attachInterrupt(0, rencoder, RISING);
  
  Serial.begin(115200);
  
  noInterrupts();           // disable all interrupts
  // initialize timer3
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;
  
  OCR3A = 625;            // compare match register 16MHz/256/100Hz
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler 
//  TCCR3B |= (1 << CS12) | (1 << CS10);    // 1024 prescaler 
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

ISR(TIMER3_COMPA_vect) {
//    detachInterrupt(0);
  Serial.println(count);
  count = 0;
}


void rencoder()                                      // pulse and direction, direct port reading to save cycles
{
  if (enPinB == LOW)    count--;                // if(digitalRead(encodPinB1)==HIGH)   count ++;
  else                  count++;   // if (digitalRead(encodPinB1)==LOW)   count --;
}

void loop()
{
  // your program here...
}
