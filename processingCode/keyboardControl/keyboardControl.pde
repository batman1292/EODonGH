int value = 0;
int speedA = 0;
int speedB = 0;
int speedLevel = 4000;
long time = millis();
int state = 0;
int numMotor = 1;

import processing.serial.*;

Serial myPort;
String ack = "";

void setup() {
  println(Serial.list());
  myPort = new Serial(this, Serial.list()[5], 38400);
}

void draw() {
  fill(value);
  rect(25, 25, 50, 50);
  if (state == 1) {
    if (millis() - time > 10) {
      time = millis();
      switch(numMotor){
        case 0: 
          myPort.write("01,M1,"+speedA+"\r");
          break;
        case 1:
          myPort.write("02,M1,"+speedB+"\r");
          break;
        case 2:
          myPort.write("03,M1,0\r");
          break;
      }
//      if(numMotor == 2){
//        numMotor = 0;
//      }else{
//        numMotor++;
//      }
    }
  }
}

void keyPressed() {
  if (keyCode == 81) {
    myPort.write("01,LS,1\r");
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
    speedA = 3000;
    speedB = 3000;
  }else if(keyCode == 68){
//    press d key
    speedA = 4500;
    speedB = 4500;
  }else if(keyCode == 68){
//    press d key
    speedA = 4500;
    speedB = 4500;
  }
  println(keyCode);
  ack = "";
}

void keyReleased() {
  speedA = 0;
  speedB = 0;
}

