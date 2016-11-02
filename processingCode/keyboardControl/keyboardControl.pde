int value = 0;
int speedA = 0;
int speedB = 0;
long time = millis();

import processing.serial.*;

// The serial port:
Serial myPort;

void setup(){
//  printArray(Serial.list());
  myPort = new Serial(this, Serial.list()[4], 38400);
}

void draw() {
  fill(value);
  rect(25, 25, 50, 50);
  if(millis() - time == 10){
    
  }
}

void keyPressed() {
  if(keyCode == 81){
    myPort.write("01,LS,1\n");
  }else if(keyCode == 87){
    myPort.write("01,LS,0\n");
  }else if(keyCode == 65){
    myPort.write("02,LS,1\n");
  }else if(keyCode == 83){
    myPort.write("02,LS,0\n");
  }else if(keyCode == 65){
    myPort.write("01,LS,1\n");
  }else if(keyCode == 83){
    myPort.write("01,LS,0\n");
  }
  println(keyCode);
}

void keyReleased() {
  speedA = 0;
  speedB = 0;
}

