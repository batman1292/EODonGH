#define InA1            10                      // INA motor pin Direction motor
#define InB1            11                      // INB motor pin Direction motor
#define PWM1            9                       // PWM motor pin
#define LOOPTIME        10                      // Loop time
#define encodPinA1      2                       // encoder A pin
#define encodPinB1      3                       // encoder B pin
////////////////////////////////////////////////////////////////////////////////
unsigned long loop_time;
unsigned long lastMilli = 0;
volatile long count = 0; 
int speed_req = 500;                             // set point speed 
int PWM_val = 0;                                // value of PWM signal to Motor drive 
int speed_act = 0;                              // actual speed (current speed)
int i = 0; 
int integral =0;
//////////////////////////////////////////////////////////////////////////////////////
float Kp =   10;                                 // PID integral control Gain
float Kd =    0;                                // PID Derivitave control gain
float Ki =    0;                                // PID Derivitave control gain
//////////////////////////////////////////////////////////////////////////////////
void setup() {
        Serial.begin(115200);                   // opens serial port, sets data rate to 115200 bps
        attachInterrupt(0, rencoder, FALLING);  // External interrupt for motor speed count at CH 0
        pinMode(encodPinA1, INPUT);
        pinMode(encodPinB1, INPUT); 
        pinMode(InA1, OUTPUT);
        pinMode(InB1, OUTPUT);
        pinMode(PWM1, OUTPUT);
        digitalWrite(InA1, LOW);
        digitalWrite(InB1, HIGH);  
}

void loop() {
            if((millis()-lastMilli) >= LOOPTIME) // check time for entry control loop
              {                                    
                lastMilli = millis();            // enter timed loop
                getMotorData();                  // go to getMotorData function
                PWM_val= updatePid(PWM_val, speed_req, speed_act);   // compute PWM value from PID controller
                analogWrite(PWM1, PWM_val);      // send PWM to motor drive
                }
             }
     

void getMotorData()                  // calculate speed, 
        {                                                        
        static long countAnt = 0;    // last count
        speed_act = ((countAnt - count)*(60))/4; // (encoder pulses X 50 (50 samplimg per sec) X 60 (1 min))/(360 (ppr))
        Serial.println(speed_act); 
        countAnt = count;                  
        }
  void rencoder()                                      // pulse and direction, direct port reading to save cycles
     {
     if (encodPinB1 == LOW)    count++;                // if(digitalRead(encodPinB1)==HIGH)   count ++;
     else                      count--;   // if (digitalRead(encodPinB1)==LOW)   count --;
      }

    int updatePid(int command, int targetValue, int currentValue)                // compute PWM value
      {
      float pidTerm = 0;                                                            // PID correction
      int error=0;                                  
      static int last_error=0; 
      error = abs(targetValue) - abs(currentValue); 
      integral = integral + (error * (LOOPTIME/1000)); 
        if(integral > 1)
        {
          integral = 0.25;
        }
      pidTerm = (Kp * error) + (Kd * (error - last_error) + (Ki * integral ));                            
      last_error = error;
      return constrain(command + int(pidTerm), 0, 255);
    }

