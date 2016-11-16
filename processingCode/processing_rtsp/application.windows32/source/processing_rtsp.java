import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import codeanticode.gsvideo.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class processing_rtsp extends PApplet {

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



GSPipeline pipeline;

public void setup() {
  size(720, 576);
  
  // VideoTestSrc pipeline. Note that there is no need to specify a 
  // video sink as the last element of the pipeline, because GSVideo
  // automatically directs the video frames of the pipeline to  
  // Processing's drawing surface.  
//  pipeline = new GSPipeline(this, "rtsp://admin:123456@192.168.0.100:7070");
  
  //pipeline = new GSPipeline(this, "rtspsrc location=rtsp://admin:123456@192.168.5.200:7070 ! decodebin ! ffmpegcolorspace !  video/x-raw-rgb,bpp=32, depth=24");
 
 pipeline = new GSPipeline(this, "rtspsrc location=rtsp://admin:123456@192.168.5.100:7070/ latency=0 ! queue ! decodebin channel=3"); 
  // The pipeline starts in paused state, so a call to the play()
  // method is needed to get thins rolling.
//  println("Pipeline string:");
//  println(pipeline.getPipeline());
  pipeline.play();
  background(255);
}

public void draw() {
  // When the GSPipeline.available() method returns true, 
  // it means that a new frame is ready to be read.
//  background(255);
  if (pipeline.available()) {
    pipeline.read();
    
    image(pipeline, 0, 0);

  }
}
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "processing_rtsp" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
