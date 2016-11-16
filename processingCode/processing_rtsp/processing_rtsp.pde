/**
 * Test. 
 * By Andres Colubri
 * 
 * This example shows how to create GStreamer pipelines using the GSPipeline object.
 * Pipelines allow to connect different gstreamer elements (video sources, decoders, etc)
 * in order to construct a video or audio stream. The command line tool gst-launch can be used
 * to launch pipelines, and most pipelines specified with gst-launch can be used in GSPipeline,
 * as the shown in this sketch. 
 * Some online material on GStreamer:
 * http://www.cin.ufpe.br/~cinlug/wiki/index.php/Introducing_GStreamer
 * http://www.twm-kd.com/computers/software/webcam-and-linux-gstreamer-tutorial/ 	
 */

import codeanticode.gsvideo.*;

GSPipeline monitor1, monitor2;
PImage img;
int chMonitor1 = 4;
int chMonitor2 = 4;

void setup() {
  size(720, 576);
//  monitor1.dispose();
//  monitor1 = new GSPipeline(this, "rtsp://admin:123456@192.168.0.100:7070/");
  monitor1 = new GSPipeline(this, "rtspsrc location=rtsp://admin:123456@192.168.5.100:7070/ latency=0 ! queue ! decodebin channel=3"); 
//  monitor1 = new GSPipeline(this, "rtspsrc location=rtsp://admin:123456@192.168.5.100:7070/ latency=0 ! queue ! decodebin channel="+chMonitor1); 
  //  if(monitor1.ready()){
  //    println("ready");
  //  }
//  monitor1.loop();
//  img = createImage(200, 200, ARGB);
  monitor1.play();
  println("data");
  println(monitor1.data);
  println("Pipeline string:");
  println(monitor1.getPipeline());
  background(255);
}

void draw() {
  // When the GSPipeline.available() method returns true, 
  // it means that a new frame is ready to be read.
  //  background(255);
  if (monitor1.available()) {
    monitor1.read();
    background(255);
    image(monitor1, 0, 0);
  }
}

void keyPressed() {
  if (keyCode == 81) {
    if (chMonitor1 == 4) {
      chMonitor1 = 1;
    } else {
      chMonitor1++;
    }
//    monitor1.pause();
    monitor1 = new GSPipeline(this, "rtspsrc location=rtsp://admin:123456@192.168.5.100:7070/ latency=0 ! queue ! decodebin channel="+chMonitor1); 
    monitor1.play();
    println("data");
    println(monitor1.data);
    println("Pipeline string:");
    println(monitor1.getPipeline());
    //    myPort.write("01,LS,1\r");q
  }
}

