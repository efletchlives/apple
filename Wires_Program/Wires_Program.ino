#include <Wire.h>
#include "ADS1X15.h"
#include "Voltage_Value.h"

ADS1115 ADS(0x48);

RangedInt RED(3700, 4700, 4200);
RangedInt GREEN(8500, 9500, 9000);
RangedInt BLACK (13400, 14500, 13950);


//BLACK(3.3V A0) => 13400 - 14500
//GREEN(2.3V A1) => 8500 - 9500
//RED (1.3v A2) => 3700-4700


void setup() {
  Serial.begin(500000);
  Wire.begin();

  ADS.begin();
  ADS.setGain(0);
  ADS.setDataRate(7);
  ADS.setMode(1);

  randomSeed(analogRead(0));
}

void wireAssign(RangedInt wires[3]) {
  bool usedRed = false;
  bool usedGreen = false;
  bool usedBlack = false;

  for (int i = 0; i < 3; i++) {
    int pick;
    bool valid = false;

    // Keep picking until we get an unused color
    while (!valid) {
      pick = random(0, 3); // 0 = RED, 1 = GREEN, 2 = BLACK

      if (pick == 0 && !usedRed) {
        wires[i] = RED;
        usedRed = true;
        valid = true;
      }
      else if (pick == 1 && !usedGreen) {
        wires[i] = GREEN;
        usedGreen = true;
        valid = true;
      }
      else if (pick == 2 && !usedBlack) {
        wires[i] = BLACK;
        usedBlack = true;
        valid = true;
      }
    }
  }
}


void loop() {
  RangedInt wires[3];
  wireAssign(wires);

  bool correct = false;
  while(!correct)
  {
    for (int i = 0; i < 3; i++) 
    {
      int reading = ADS.readADC(i);
      if (!wires[i].isInRange(reading)) {
        correct = false;
      }
    }
  }

  if (correct) {
    Serial.println("✅ CORRECT ORDER!");
    while (true) {}
  } else {
    Serial.println("❌ Wrong order, try again...");
    delay(1000);
  }
}
