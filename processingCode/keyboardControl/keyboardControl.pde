int value = 0;
int speedA = 0;
int speedB = 0;
int speedLevel = 50;
long time = millis();
int state = 0;
boolean cmdMotor = false;

import processing.serial.*;

// The serial port:
Serial myPort;
String ack = "";

void setup() {
  //  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[4], 38400);
}

void draw() {
  fill(value);
  rect(25, 25, 50, 50);
  if (state == 2) {
    //    println(millis() - time);
    if (millis() - time > 5) {
      time = millis();
      //      println(speedA);
      //      myPort.write("02,MO,"+speedA+"\n");
      if (cmdMotor) {
        myPort.write("01,MO,"+speedA+"\n");
      } else {
        myPort.write("02,MO,"+speedB+"\n");
//        cmdMotor = true;
      }
      ack = myPort.readStringUntil('\n');
      if (ack != null) {
        cmdMotor = !cmdMotor;
      }
      ack = "";
      //      delay(5);z
      //      print("02,MO,"+speedB+"\n");
    }
  }
}

void keyPressed() {
  if (keyCode == 81) {
    myPort.write("03,LS,1\n");
  } else if (keyCode == 87) {
    myPort.write("03,LS,0\n");
  } else if (keyCode == 65) {
    myPort.write("02,LS,1\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 83) {
    myPort.write("02,LS,0\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 90) {
    myPort.write("01,DT,\n");
    delay(5);
    myPort.write("02,DT,\n");
    delay(5);
    state = 0;
  } else if (keyCode == 88) {
    myPort.write("01,CT,\n");
    delay(5);
    myPort.write("02,CT,\n");
    delay(5);
    state = 2;
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
  }

  ack = "";
  //  println(keyCode);
}

void keyReleased() {
  speedA = 0;
  speedB = 0;
}

