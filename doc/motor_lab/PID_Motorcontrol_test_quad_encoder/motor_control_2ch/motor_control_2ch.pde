/* read a rotary encoder with interrupts
   Encoder hooked up with common to GROUND,
   encoder0PinA to pin 2, encoder0PinB to pin 4 (or pin 3 see below)
   it doesn't matter which encoder pin you use for A or B  

   uses Arduino pullups on A & B channel outputs
   turning on the pullups saves having to hook up resistors 
   to the A & B channel outputs 

*/ 

#define encoder0PinA  2
#define encoder0PinB  3

#define SpeedPin     9
#define DirectionPin 8

volatile long encoder0Pos = 0;
long target = 0;

//correction = Kp * error + Kd * (error - prevError) + kI * (sum of errors)
//PID controller constants
float KP = 2.0; //position multiplier (gain)
float KI = .25; // Intergral multiplier (gain)
float KD = 0; // derivative multiplier (gain)

int lastError = 0;
int sumError = 0;
//Integral term min/max (random value and not yet tested/verified)
int iMax = 100;
int iMin = 0;

long previousTarget = 0;
long previousMillis = 0;        // will store last time LED was updated
long interval = 10;           // interval at which to blink (milliseconds)

void setup() { 

  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 
  
  pinMode(DirectionPin, OUTPUT); 
  pinMode(SpeedPin, OUTPUT); 

  attachInterrupt(0, doEncoderA, CHANGE);  // encoder pin on interrupt 0 - pin 2
  attachInterrupt(1, doEncoderB, CHANGE);  
  
  Serial.begin (115200);
  Serial.println("start");                // a personal quirk

} 

void loop(){
  
  
  // do some stuff here - the joy of interrupts is that they take care of themselves  
  if (millis() - previousTarget > 1000)
  {
      Serial.println ( target );
      
      previousTarget = millis();
      //target += 1024;
      
      target += 2048;
  }
  
  docalc();
}


void docalc() {
  
  if (millis() - previousMillis > interval) 
  {
    previousMillis = millis();   // remember the last time we blinked the LED
    
    long error = encoder0Pos - target ; // find the error term of current position - target    
    
    //generalized PID formula
    //correction = Kp * error + Kd * (error - prevError) + kI * (sum of errors)
    long ms = KP * error + KD * (error - lastError) +KI * (sumError);
    
    //Serial.print ( ms );
    //Serial.print ( " , " );
    //Serial.println ( encoder0Pos );
    
    lastError = error;    
    sumError += error;
    
    //scale the sum for the integral term
    if(sumError > iMax) {
      sumError = iMax;
    } else if(sumError < iMin){
      sumError = iMin;
    }
    
    if(ms > 0){
      digitalWrite ( DirectionPin ,HIGH );      
    }
    if(ms < 0){
      digitalWrite (DirectionPin , LOW );
      ms = -1 * ms;
    }

    int motorSpeed = map(ms,0,1024,0,255); 
    analogWrite ( SpeedPin, motorSpeed);
 
  }
  
  
}
void doEncoderA(){
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

void doEncoderB(){

  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {   

    // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == LOW) {  
      encoder0Pos = encoder0Pos + 1;         // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;         // CCW
    }
  }

  // Look for a high-to-low on channel B

  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinA) == HIGH) {   
      encoder0Pos = encoder0Pos + 1;          // CW
    } 
    else {
      encoder0Pos = encoder0Pos - 1;          // CCW
    }
  }

} 

