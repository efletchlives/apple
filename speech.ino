#include <Wire.h>
#include "DFRobot_ASR.h"

DFRobot_ASR asr;

void setup() {
  Serial.begin(115200);

  int working = asr.begin(eMode_LOOP, eMicrophoneMode_MIC)
  if (working != 0) {
    Serial.println("speech library init failed.");
  }
  else {
    Serial.println("speech library initialized!")
  }
  asr.addCommand("apple", 1);
  asr.addCommand("orange", 2);

  
  asr.start();
}

void loop() {
  int id = asr.read();

  if (id != 0) {
    Serial.print("ID = ");
    Serial.print(id);

    switch (id) {
      case 1:
        Serial.println("you said apple");
        break;

      case 2:
        Serial.println("you said orange");
        break;
      
      default:
        Serial.println("unknown");
        break;
    }
  }
  delay(100);
}
