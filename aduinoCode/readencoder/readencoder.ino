#define enPinA  2                               // encoder A pin
#define enPinB  3                               // encoder B pin
#define enPinO  9
int speed_act = 0;                              // actual speed (current speed)

volatile long count = 0;

void rencoder(){                                // pulse and direction, direct port reading to save cycles{
  if (enPinB == LOW)    count++;                // if(digitalRead(encodPinB1)==HIGH)   count ++;
  else                  count--;
  digitalWrite(enPinO, digitalRead(enPinO) ^ 1);    
}

ISR(TIMER3_COMPA_vect)                          // timer compare interrupt service routine
{
  static long countAnt = 0;                     // last count
  speed_act = ((countAnt - count) * (60)) / 5;  // (encoder pulses X 100 (100 samplimg per sec) X 60 (1 min))/(360 (ppr))
//  Serial.println(speed_act);                  // print speed (rpm)
  Serial.println((countAnt - count) * 100);     // print count per sec
  countAnt = count;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  //init encoder pin
  pinMode(enPinA, INPUT); 
  digitalWrite(enPinA, HIGH);       // turn on pull-up resistor
  pinMode(enPinB, INPUT); 
  digitalWrite(enPinB, HIGH);       // turn on pull-up resistor

  attachInterrupt(0, rencoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

  pinMode(enPinO, OUTPUT);
  digitalWrite(enPinO, HIGH);
  // initialize timer3
  noInterrupts();           // disable all interrupts
  TCCR3A = 0;
  TCCR3B = 0;
  TCNT3  = 0;

  OCR3A = 625;              // compare match register 16MHz/256/100Hz 
  TCCR3B |= (1 << WGM12);   // CTC mode
  TCCR3B |= (1 << CS12);    // 256 prescaler 
  TIMSK3 |= (1 << OCIE3A);  // enable timer compare interrupt
  interrupts();             // enable all interrupts
}

void loop() {
  // put your main code here, to run repeatedly:

}
