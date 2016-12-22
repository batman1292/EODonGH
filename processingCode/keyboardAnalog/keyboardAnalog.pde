import processing.serial.*;
import controlP5.*;

ControlP5 cp5;
Knob myKnobA;
Knob myKnobB;
Knob myKnobC;

long time = millis();

Serial myPort;
int state = 0;
int id = 1;


void setup() {
  myPort = new Serial(this, Serial.list()[4], 38400);
  myPort.clear();
  size(600, 400);
  background(0);
  textSize(26); 
  text("RS485 Analog Reader", 175, 75); 
  
  cp5 = new ControlP5(this);

  myKnobA = cp5.addKnob("AnalogID1")
               .setRange(0, 1023)
               .setValue(0)
               .setPosition(50, 125)
               .setRadius(75);
//               .setDragDirection(Knob.VERTICAL);
               
  myKnobB = cp5.addKnob("AnalogID2")
               .setRange(0, 1023)
               .setValue(0)
               .setPosition(225, 125)
               .setRadius(75);
//               .setDragDirection(Knob.VERTICAL);
               
  myKnobC = cp5.addKnob("AnalogID3")
               .setRange(0, 1023)
               .setValue(0)
               .setPosition(400, 125)
               .setRadius(75);
//               .setDragDirection(Knob.VERTICAL);
               
}

void draw() {
  if (state==1) {
    if (millis() - time > 20) {
      time = millis();
      myPort.write("0"+id+",AN\r");
      String inString = myPort.readStringUntil('\n');
      if (inString != null) {
        float value = Float.parseFloat(split(inString, ',')[2]);
        if (id == 3) {
          myKnobC.setValue((value));
          id = 1;
        } else {
          if(id == 1)
            myKnobA.setValue((value));
          else
            myKnobB.setValue((value));
          id++;
        }
      }
      //      myPort.clear();
    }
  }
}

void keyPressed() {
  //key press for turn on/off LED
  if (keyCode == 81) {                    //q
    myPort.write("01,LS,1\r");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 87) {             //w
    myPort.write("01,LS,0\r");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 65) {             //a 
    myPort.write("02,LS,1\r");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 83) {             //s
    myPort.write("02,LS,0\r");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 90) {             //z
    myPort.write("03,LS,1\r");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 88) {             //x
    myPort.write("03,LS,0\r");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 74) {
    state = 1;
  } else if (keyCode == 75) {
    state = 0;
    id=1;
  }
  //  print(ack);
  ack = "";
  myPort.clear();
  println(keyCode);
}

