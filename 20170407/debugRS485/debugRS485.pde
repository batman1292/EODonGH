String myString = null;
import processing.serial.*;

Serial myPort;
  
PrintWriter output;

void setup() {
//  println(Serial.list());
  myPort = new Serial(this, "COM40", 38400);
  output = createWriter("sniff485_Test.txt"); 
}

void draw() {
  myString = myPort.readStringUntil(13);
  if (myString != null) {
//    println(myString);
     output.println(myString);
  }
}

void keyPressed() {
  output.flush(); // Writes the remaining data to the file
  output.close(); // Finishes the file
  exit(); // Stops the program
}
