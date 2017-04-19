String myString = null;
import processing.serial.*;

Serial myPort;
String ack = "";

long time = millis();
int countMotor = 0;
int numMotor = 3;
boolean isSend = false;
int Intervalms = 10;
int timeOut = 40;
int countProcess = 0;
int speed = 0;

void setup() {
  myPort = new Serial(this, "COM11", 38400);
}

void draw() {
  if (millis() - time > Intervalms) {
    time = millis();
    myPort.write("01,M1,2000\r");
    while (millis () - time < timeOut) {
      myString = myPort.readStringUntil(13);
      if (myString != null) {
//        println(myString);
        break;
      }
    }
    time = millis();
    countMotor++;
  }
}

