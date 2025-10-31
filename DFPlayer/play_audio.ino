#include "DFRobotDFPlayerMini.h"

// Define your chosen pins (you can use almost any GPIO)
#define RXD2 18  // Connect to DFPlayer TX
#define TXD2 17  // Connect to DFPlayer RX (through 1k resistor)

DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(115200);
  
  // Initialize Serial1 or Serial2 with custom pins
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  
  myDFPlayer.begin(Serial1);
  
  myDFPlayer.volume(20);  // Set volume (0-30)
  myDFPlayer.play(1);     // plays 000#.mp3
}

void loop() {
  // Your code here
}
