// key board control EOD track robot with RS485
// this code had check ack from Ardu485 
// and set timeout at 50 ms for next command if Ardu485 dosen't send ack

String myString = null;
import processing.serial.*;

Serial myPort;
String ack = "";

long time = millis();
int countMotor = 0;
int numMotor = 3;
boolean isSend = false;
int Intervalms = 10;
int timeOut = 50;
int countProcess = 0;
int speed = 0;
int speedLevel = 3000;

void setup() {
  myPort = new Serial(this, "COM11", 38400);
}

void draw() {
  if (millis() - time > Intervalms) {
    time = millis();
    myPort.write("0"+(countMotor%numMotor+1)+",M1,"+speed+"\r");
    while (millis () - time < timeOut) {
      myString = myPort.readStringUntil(13);
      if (myString != null && int(myString.charAt(1)) == char(countMotor%numMotor+49)) {
//        println(myString);
        break;
      }
    }
    time = millis();
    countMotor++;
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
  }
}

void keyReleased() {
  speed = 0;
//  speedB = 0;
}

