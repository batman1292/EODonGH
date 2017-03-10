/*  
Test Quadrature Encoder by use one external interrupt


to read the other two transitions - just use another one of signal to attachInterrupt()
in the setup 

You need to use one encoder wire to pin 4 and anthor encoder wire to pin 2 (interrupt 1).
*/ 

#define encoder0PinA  2
#define encoder0PinB  4

volatile unsigned int encoder0Pos = 0;

long previousMillis = 0;        // will store last time tick updated
long interval = 1000;           // interval at which to tick (milliseconds)


void setup() { 


  pinMode(encoder0PinA, INPUT); 
  digitalWrite(encoder0PinA, HIGH);       // turn on pullup resistor
  pinMode(encoder0PinB, INPUT); 
  digitalWrite(encoder0PinB, HIGH);       // turn on pullup resistor

  attachInterrupt(0, doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2
  Serial.begin (9600);
  Serial.println("start");                // a personal quirk

} 

void loop(){
	// do some stuff here - the joy of interrupts is that they take care of themselves
	docalc();	
}


void docalc() {
  if (millis() - previousMillis > interval) {
    previousMillis = millis();   // remember the last time we tick signals
    
	Serial.print ( previousPos - encoder0Pos ,DEC);
    Serial.print ( " , " );
    Serial.println ( encoder0Pos ,DEC);
    
    previousPos = encoder0Pos;
	    
  }
  
  
}
void doEncoder(){
  if (digitalRead(encoder0PinA) == HIGH) {   // found a low-to-high on channel A
    if (digitalRead(encoder0PinB) == LOW) {  // check channel B to see which way
                                             // encoder is turning
      encoder0Pos = encoder0Pos - 1;         // CCW
    } 
    else {
      encoder0Pos = encoder0Pos + 1;         // CW
    }
  }
  else                                        // found a high-to-low on channel A
  { 
    if (digitalRead(encoder0PinB) == LOW) {   // check channel B to see which way
                                              // encoder is turning  
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }

  }
  
}
