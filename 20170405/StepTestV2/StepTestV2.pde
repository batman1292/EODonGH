// key board control EOD track robot with RS485
// this code had check ack from Ardu485 
// and set timeout at 50 ms for next command if Ardu485 dosen't send ack

String myString = null;
import processing.serial.*;

Serial myPort;
String ack = "";

long time = millis();
int numMotor = 3;
int Intervalms = 10;
int timeOut = 50;
int speed = 0;
int speedLevel = 3000;
int delayTime = 3000/timeOut;
int testStates = 8;
int [] stepSpeedList = {0, 1500, 3000, 4500, 0, -1500, -3000, -4500};

int countMotor = 0;
int countTestStates = 0;
int countTime = 1;
boolean isStart = false;

void setup() {
  myPort = new Serial(this, "COM10", 38400);
}

void draw() {
  if(isStart){
    if (millis() - time > Intervalms) {
      time = millis();
      // check time and init speed for test
      countTime = (countTime+1)%delayTime;
      if(countTime == 0) countTestStates++;
      speed = stepSpeedList[countTestStates % testStates];
      println("0"+(countMotor%numMotor+1)+",M1,"+speed+"\r");
  //    myPort.write("0"+(countMotor%numMotor+1)+",M1,"+speed+"\r");
      while (millis () - time < timeOut) {
        myString = myPort.readStringUntil(13);
        if (myString != null && int(myString.charAt(0)) == char(countMotor%numMotor+49)) {
          break;
        }
      }
      time = millis();
      countMotor++;
    }
  }
}
void keyPressed() {
  if (keyCode == 38) {
    speed = speedLevel;
//    speedB = speedLevel;
  } else if (keyCode == 40) {
    speed = -speedLevel;
//    speedB = -speedLevel;
  } else if (keyCode == 37) {
    myPort.write("01,LS,1");
    delay(100);
    myPort.write("02,LS,1");
  } else if (keyCode == 39) {
    myPort.write("01,LS,0");
    delay(100);
    myPort.write("02,LS,0");
//    speedA = speedLevel;
//    speedB = -speedLevel;
  }else if (keyCode == 90) {
//    press z key to stop
    isStart = false;
    countMotor = 0;
    countTestStates = 0;
    countTime = 1;
  } else if (keyCode == 88) {
//    press x key to start
    isStart = true;
  }
}

void keyReleased() {
  speed = 0;
//  speedB = 0;
}

