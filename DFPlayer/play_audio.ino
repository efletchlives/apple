#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial soft_serial(10,11);
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  soft_serial.begin(9600);
  Serial.begin(9600);

  myDFPlayer.begin(soft_serial);
  myDFPlayer.volume(25); // 0-30
  myDFPlayer.play(1); // or whatever the wav file is named

  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
