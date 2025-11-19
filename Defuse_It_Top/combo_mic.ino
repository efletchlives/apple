// HW-484 Sound Sensor Test for ESP32-S3
#include <Wire.h>
#include "ADS1X15.h"
#include "Voltage_Value.h"
#include <Adafruit_SSD1306.h>
// // Pin definitions
// #define ANALOG_PIN 2   // AO - Analog output (ADC1_CH0)
// #define DIGITAL_PIN 42   // DO - Digital output

// // Variables
// int analogValue = 0;
// int digitalValue = 0;
// int threshold = 3500;  // Adjust based on your readings (0-4095 for ESP32)
extern RangedInt WIRE_VOID; // void/open wires


void micSetup() {
  Serial.begin(115200);
  
  // Configure pins
  pinMode(DIGITAL_PIN, INPUT);
  pinMode(ANALOG_PIN, INPUT);

  analogReadResolution(12);
  analogSetAttenuation(ADC_11db);
  delay(1000);
}

bool micLoop(double timer,bool &wire_in) {

  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE);
  Display1.setCursor(10,25);
  Display1.println(F("PANIC"));

  Display1.display();
  
  unsigned long startTime = millis();
  unsigned long currentTime = startTime;
  while((currentTime-startTime) <= timer)
  {
    // check for wrong input
    // if wires values change 
     ADS.readADC(0);
      int16_t readingA0 = ADS.getValue();
       ADS.readADC(1);
      int16_t readingA1 = ADS.getValue();
       ADS.readADC(2);
      int16_t readingA2 = ADS.getValue();
          //First check if wires are in 
            //No wires are in 
          if(wire_in == 0)
          {
            //Checks if any wires get plugged in
            if(!WIRE_VOID.isInRange(readingA0) && !WIRE_VOID.isInRange(readingA1) && !WIRE_VOID.isInRange(readingA2))
            {
              return false;
            }
          }
          else if(wire_in == 1)
          {
            //Checks if any wires were removed
            if(WIRE_VOID.isInRange(readingA0) && WIRE_VOID.isInRange(readingA1) && WIRE_VOID.isInRange(readingA2))
            {
              return false;
            }
          }


    // if key is pressed on keypad
    char key = keypad.getKey();
      if(key){
        return false; // wrong input
      }

    
    // Read analog value (0-4095 on ESP32)
    analogValue = analogRead(ANALOG_PIN);
    
    // Read digital value (HIGH when sound detected)
    digitalValue = digitalRead(DIGITAL_PIN);
    
    //Print values
    Serial.print("Analog: ");
    Serial.print(analogValue);
    Serial.println();
     Serial.print("\t|\tDigital: ");
     Serial.print(digitalValue);
    
    // Check if sound detected
    // if (digitalValue == HIGH) {
    //   // Serial.println("\t<-- SOUND DETECTED (Digital)");
    //   return true;
    // } 
    if (analogValue > threshold) {
      Serial.println("\t<-- SOUND DETECTED (Analog)");
      return true;
    } 
    currentTime = millis();
      // once past certain time running, time out and return false
      // Serial.println();
    delay(100);  // Update every 100ms
  }
  return false;  
}
