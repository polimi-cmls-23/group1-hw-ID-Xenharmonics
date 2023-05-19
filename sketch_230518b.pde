import processing.serial.*;
import oscP5.*;
import netP5.*;

Serial mySerial;
OscP5 oscP5;
NetAddress myRemoteLocation;

void setup() {
  size(400, 300);

  // Open the serial port
  mySerial = new Serial(this, "/dev/cu.usbmodem1421", 9600);

  // Create OscP5 object for receiving OSC messages
  oscP5 = new OscP5(this, 8001);

  // Set the remote address and port for sending OSC messages
  myRemoteLocation = new NetAddress("127.0.0.1", 8000);

  // Start the serial reading thread
  new SerialReaderThread().start();
    
}

void draw() {
  // Nothing to draw in this example
}

void oscEvent(OscMessage message) {
  // Print the contents of the received OSC message
  printOSCMessage(message);
}

void printOSCMessage(OscMessage message) {
  String address = message.addrPattern();
  print("Received OSC message: " + address + " ");

  for (int i = 0; i < message.arguments().length; i++) {
    Object arg = message.arguments()[i];

    if (arg instanceof Integer) {
      print((int)arg + " ");
    } else if (arg instanceof Float) {
      print((float)arg + " ");
    } else if (arg instanceof String) {
      print((String)arg + " ");
    } else {
      print(arg + " ");
    }
  }
  println();
}

class SerialReaderThread extends Thread {
  public void run() {
    while (true) {
      if (mySerial != null && mySerial.available() > 0) {
        // Read serial data
        String data = mySerial.readStringUntil('\n');
        if (data != null) {
          // Create an OSC message with the received data
          OscMessage message = new OscMessage("/data");
          message.add(data.trim());

          // Send the OSC message to the remote location
          oscP5.send(message, myRemoteLocation);
          
          // Print the OSC message 
          oscEvent(message);
        }
      }
    }
  }
}
