import processing.serial.*;

int [] an = new int [3];
long time = millis();

Serial myPort;
String ack = "";
int [] ackArdu485 = {0, 0, 0};

void setup(){
  myPort = new Serial(this, Serial.list()[4], 38400);
  myPort.clear();
}

void draw(){
}

void keyPressed(){
  //key press for turn on/off LED
  if (keyCode == 81) {
    myPort.write("01,LS,1\n");ack = myPort.readStringUntil('\n');
  } else if (keyCode == 87) {
    myPort.write("01,LS,0\n");ack = myPort.readStringUntil('\n');
  } else if (keyCode == 65) {
    myPort.write("02,LS,1\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 83) {
    myPort.write("02,LS,0\n");
    ack = myPort.readStringUntil('\n');
  } else if (keyCode == 90) {
    myPort.write("03,LS,1\n");ack = myPort.readStringUntil('\n');
  } else if (keyCode == 88) {
    myPort.write("03,LS,0\n");ack = myPort.readStringUntil('\n');
  } else if (keyCode == 74){
    myPort.write("01,CT\n");
    ack = myPort.readStringUntil('\n');
//    manageAck("CT");
    delay(10);
    print(ack);
    myPort.write("02,CT\n");
    ack = myPort.readStringUntil('\n');
//    manageAck("CT");
    delay(10);
    print(ack);
    myPort.write("03,CT\n");
    ack = myPort.readStringUntil('\n');
//    manageAck("CT");
    delay(10);
    print(ack);
  }
  print(ack);
  ack = "";
  myPort.clear();
//  println(keyCode);
}

void manageAck(String cmd){
  if(cmd == "CT"){
    if(ack != null){
      String[] ackList = split(ack, ',');
      ackArdu485[Integer.parseInt(ackList[0])]++;
      print(split(ack, ','));
//      ack = "";
    }
  }
  ack = "";
}
