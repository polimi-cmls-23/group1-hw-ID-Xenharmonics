import processing.serial.*;
import oscP5.*;
import netP5.*;

Serial port;
OscP5 oscP5;
NetAddress myRemoteLocation;
PImage bg;
String data;     // Data received from the serial port
float pitch, roll, prevPitch=0, prevRoll=0, dp, dr, easing=0.06;
float numbers[] = new float[2];

void setup() {
  
  size (500, 500, P3D);
  noStroke();
  textSize(20);
  
  bg = loadImage("kineticHarm.png");
  
  // Set serial port
  port = new Serial(this, "/dev/cu.usbmodem1421", 9600);
  
  // Create OscP5 object for receiving OSC messages
  oscP5 = new OscP5(this, 8001);
  
  // Set the remote address and port for sending OSC messages
  myRemoteLocation = new NetAddress("127.0.0.1", 8000);
  
}

void draw() {
  
  translate(width/2, height/2, 0);
  background(bg);


  // Rotate the object
  rotateX(radians(prevRoll));
  rotateZ(radians(-prevPitch));
  dp = pitch - prevPitch;
  dr = roll - prevRoll;
  prevPitch += dp*easing;
  prevRoll += dr*easing;
  // 3D 0bject
  textSize(30);  
  fill(132,33,123);
  box (300, 30, 100); // Draw box
  textSize(25);
  fill(255, 200, 200);
  text("Xenharmonics", -100, 10, 101);

  if (port.available() > 0) {
    data = port.readStringUntil('\n');
    if (data != null) {
        // split the string at " "
        String items[] = split(data, ' ');
      
        if (items.length > 4) { 
                
          // Compose the 3 axis parameters
          String axis_parameters = items[0]+ " " + items[1]+ " " +  items[2]+ " ";
        
          // Create the OSC Message
          OscMessage message = new OscMessage("/data");
          message.add(axis_parameters);
              
          // Send the OSC message to the remote location
          oscP5.send(message, myRemoteLocation);
        
          // Roll,Pitch in degrees
          roll = float(items[3]);
          pitch = float(items[4]);
         }
     }
  }
}
