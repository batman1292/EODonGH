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
int speed = 2000;

void setup() {
  myPort = new Serial(this, "COM11", 38400);
}

void draw() {
  if (millis() - time > Intervalms) {
    time = millis();
    myPort.write("0"+(countMotor%numMotor+1)+",M1,"+speed+"\r");
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

