import processing.serial.*;

Serial myPort;
PrintWriter data;

void setup(){
  myPort = new Serial(this, Serial.list()[4], 115200);
  data = createWriter("motorProfile.txt"); 
}

void draw(){
  while (myPort.available() > 0) {
    String inBuffer = myPort.readString();   
    if (inBuffer != null) {
      println(inBuffer);
      data.println(inBuffer);
    }
  }
}
