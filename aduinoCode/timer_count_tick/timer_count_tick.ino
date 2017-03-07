#define ledPin 9
#define enPinA  2                               // encoder A pin
#define enPinB  3                               // encoder B pin
int encoder0PinALast = LOW;
int speed_act = 0;                              // actual speed (current speed)
uint8_t enable = 0;
int countLoop = 0;

volatile long encoder0Pos = 0;
int n = LOW;

void setup()
{
  pinMode(ledPin, OUTPUT);
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
  // set compare match register for 2khz increments
  OCR0A = 40;// = (16*10^6) / (2000*64) (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |=  (1 << CS01);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);
  interrupts();             // enable all interrupts
}

ISR(TIMER3_COMPA_vect)          // timer compare interrupt service routine
{
//  TIMSK0 |= (1 << OCIE0A);
// Serial.print (millis()-lastMilli);
//      Serial.println ("/");
  enable = 1;
  Serial.print (encoder0Pos);
  Serial.println ("/");
//  digitalWrite(ledPin, HIGH);
//  digitalWrite(ledPin, digitalRead(ledPin) ^ 1);   // toggle LED pin
}

ISR(TIMER0_COMPA_vect) { 
  if(enable == 1){
    if(countLoop < 200){
//      digitalWrite(ledPin, HIGH);
      n = digitalRead(enPinA);
//      Serial.print(digitalRead(enPinA));      Serial.print('/');
//      Serial.print(digitalRead(enPinB));  Serial.println();
      if ((encoder0PinALast == LOW) && (n == HIGH)) {
        if (digitalRead(enPinB) == LOW) {
          encoder0Pos--;
        } else {
          encoder0Pos++;
        }
      } 
      encoder0PinALast = n;
      countLoop++;
//      digitalWrite(ledPin, LOW);
    }else{
//      digitalWrite(ledPin, LOW);
      countLoop = 0;
      enable = 0;
    }
  }
}

void loop()
{
  // your program here...
}
