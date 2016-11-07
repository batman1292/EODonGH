import processing.serial.*;

int [] an = new int [3];
long time = millis();

Serial myPort;
String ack = "";
int [] ackArdu485 = {
  0, 0, 0
};
int state = 0;
int id = 1;
int xPos = 1;
int lastxPos=1;
int [] lastheight = {
  0, 0, 0
};

void setup() {
  myPort = new Serial(this, Serial.list()[4], 115200);
  myPort.clear();
  size(600, 400);
}

void draw() {
  if (state==1) {
    if (millis() - time > 20) {
      time = millis();
      myPort.write("0"+id+",AN\n");
      String inString = myPort.readStringUntil('\n');
      if (inString != null) {
        float inByte = Float.parseFloat(split(inString, ',')[2]);
        inByte = map(inByte, 0, 1023, 0, height); //map to the screen height.

        //Drawing a line from Last inByte to the new one.

        stroke(85*id, 85*id, 85*id);     //stroke color
        strokeWeight(2);        //stroke wider
        line(lastxPos, lastheight[id-1], xPos, height - inByte); 
        lastxPos= xPos;
        lastheight[id-1] = int(height-inByte);
        if (id == 1) {
          // at the edge of the window, go back to the beginning:
          if (xPos >= width) {
            xPos = 0;
            lastxPos= 0;
            background(0);  //Clear the screen.
          } else {
            // increment the horizontal position:
            xPos++;
          }
          id = 1;
        }else{
          id++;
        }
      }
      //      myPort.clear();
    }
  }
}

void keyPressed() {
  //key press for turn on/off LED
  if (keyCode == 81) {
    myPort.write("01,LS,1\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 87) {
    myPort.write("01,LS,0\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 65) {
    myPort.write("02,LS,1\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 83) {
    myPort.write("02,LS,0\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 90) {
    myPort.write("03,LS,1\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 88) {
    myPort.write("03,LS,0\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 74) {
    state = 1;
  } else if (keyCode == 75) {
    state = 0;
  }
  //  print(ack);
  ack = "";
  myPort.clear();
  //  println(keyCode);
}

void manageAck(String cmd) {
  if (cmd == "CT") {
    if (ack != null) {
      String[] ackList = split(ack, ',');
      ackArdu485[Integer.parseInt(ackList[0])]++;
      print(split(ack, ','));
      //      ack = "";
    }
  }
  ack = "";
}

