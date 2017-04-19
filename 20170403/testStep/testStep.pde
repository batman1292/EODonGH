int value = 0;
int speedA = 0;
int speedB = 0;
int speedLevel = 2000;
long time = millis();
int state = 0;

int testState = 8;
int testStateCount = 0;
int countTime = 1;
int delayTime = 60;
int interval_ms = 20;
int speedMotor = 1500;

String myString = null;
import processing.serial.*;

Serial myPort;
String ack = "";

void setup() {
  println(Serial.list());
  myPort = new Serial(this, "COM11", 38400);
}

void draw() {
  myString = myPort.readStringUntil(10);
  if (myString != null) {
    println(myString);
  }
  fill(value);
  rect(25, 25, 50, 50);
  if (state == 1) {
    if (millis() - time > interval_ms) {
      time = millis();
      countTime = (countTime+1)%delayTime;
//      println(countTime);
      if (countTime == 0) {
        testStateCount++;
      }
      switch(testStateCount % testState) {
      case 0:
        speedA = 0;
        speedB = 0;
        //        myPort.write("01,M1,0\r");
        break;
      case 1:
        speedA = 1500;
        speedB = 1500;
        //        myPort.write("01,M1,1500\r");
        break;
      case 2:
        speedA = 3000;
        speedB = 3000;
        //        myPort.write("01,M1,3000\r");
        break;
      case 3:
        speedA = 4500;
        speedB = 4500;
        //        myPort.write("01,M1,4500\r");
        break;
      case 4:
        speedA = 0;
        speedB = 0;
//        myPort.write("01,M1,0\r");
        break;
      case 5:
        speedA = -1500;
        speedB = -1500;
        //        myPort.write("01,M1,-1500\r");
        break;
      case 6:
        speedA = -3000;
        speedB = -3000;
        //        myPort.write("01,M1,-3000\r");
        break;
      case 7:
        speedA = -4500;
        speedB = -4500;
        //        myPort.write("01,M1,-4500\r");
        break;
      }
//      println("0"+(countTime%2+1)+",M1,"+speedA+"\r");
//      if(countTime%2 == 0){
        myPort.write("01,M1,"+speedA+"\r");
//      }else{
//        myPort.write("02,M1,"+speedB+"\r");
//      }
//      myPort.write("0"+(countTime%2+1)+",M1,"+speedA+"\r");
    }
  }
}

void keyPressed() {
  if (keyCode == 81) {
    myPort.write("01,LS,1\r");
    //    myPort.write("01,VR\r");
  } else if (keyCode == 87) {
    myPort.write("01,LS,0\r");
  } else if (keyCode == 90) {
    //    press z key
    //    myPort.write("01,DT,\n");
    //    delay(5);
    //    myPort.write("02,DT,\n");
    //    delay(5);
    state = 0;
  } else if (keyCode == 88) {
    //    press x key
    //    myPort.write("01,CT,\n");
    //    delay(5);
    //    myPort.write("02,CT,\n");
    //    delay(5);
    state = 1;
  } else if (keyCode == 38) {
    speedA = speedLevel;
    speedB = speedLevel;
    //    println(speedA);
  } else if (keyCode == 40) {
    speedA = -speedLevel;
    speedB = -speedLevel;
  } else if (keyCode == 37) {
    speedA = -speedLevel;
    speedB = speedLevel;
  } else if (keyCode == 39) {
    speedA = speedLevel;
    speedB = -speedLevel;
  } else if (keyCode == 65) {
    speedA = 6400;
    speedB = 1500;
  } else if (keyCode == 83) {
    //    press s key
    myPort.write("02,LS,1\r");
//    speedA = 3000;
//    speedB = 3000;
  } else if (keyCode == 68) {
    //    press d key
    myPort.write("02,LS,0\r");
//    speedA = 4500;
//    speedB = 4500;
  }
  myPort.clear();
  println(keyCode);
  ack = "";
}

void keyReleased() {
  speedA = 0;
  speedB = 0;
}

