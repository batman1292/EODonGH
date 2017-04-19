String myString = null;
import processing.serial.*;

Serial myPort;
PrintWriter output;

void setup() {
  myPort = new Serial(this, "COM39", 38400);
  output = createWriter("sniff485_M1_M2_stepTest_withSend.txt"); 
}

void draw(){
  myString = myPort.readStringUntil(13);
  if(myString != null){
//    if(myString.charAt(0) != '0'){
      output.println(myString);
//      println(myString);
//    }
  }
}

void keyPressed() {
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  exit(); // Stops the program
}
