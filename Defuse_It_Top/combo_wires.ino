// Evan Fletcher can confirm that this code is PRISTINE!!!!!
#include <Wire.h>
#include "ADS1X15.h"
#include "Voltage_Value.h"
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
//Adafruit_SSD1306 Display1(OLED_RESET);
//Adafruit_SSD1306 Display2(OLED_RESET);

//ADS1115 ADS(0x48);

RangedInt WIRE_GREEN(10000, 14000, 12000); // on breadboard: GREEN
RangedInt WIRE_BLUE(5000, 9000, 7000);   // on breadboard: BLUE
RangedInt WIRE_BLACK(15000, 20000, 17500); // on breadboard: BLACK
RangedInt WIRE_VOID(1000, 4500, 3200); // void/open wires
RangedInt expected_value;

// key: Green -> 0, Blue -> 1, Black -> 2
int combos[6][3] = {{0,1,2},  // [i][1] = Wire # to A0,                   
                    {0,2,1},  // [i][2] = Wire # to A1, 
                    {1,0,2},  // [i][3] = Wire # to A2
                    {1,2,0},
                    {2,0,1},
                    {2,1,0}};

int prev_combo_idx = -1; // store previous combo idx
bool done = false; // for wires display

enum wire_colors {
  green = 1,
  blue,
  black
};

String toString(int color) {
  switch(color) {
    case 0: return "green";
    case 1: return "blue";
    case 2: return "black";
  }
}


void setupWires() {
  Serial.begin(9600);

  //Display Code
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Display1.clearDisplay();
  Display1.display();

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Display2.clearDisplay();
  Display2.display();

  //Wire code
  Wire.begin(8,9);
  ADS.begin();
  ADS.setGain(0);
  ADS.setDataRate(7);
  ADS.setMode(1);
  randomSeed(analogRead(0));
}


bool wiresLoop(double timer, bool &wire_in) {

  // see if wires are already plugged in
  if(wire_in == 1){
    Display1.clearDisplay();                     //          Red -> A0
    Display1.setTextSize(0.75);                  //          Green -> A1
    Display1.setTextColor(WHITE, BLACK);         //          Yellow -> A2
    Display1.setCursor(0,0); 
    Display1.println("Remove the wires");
    Display1.display();

    speaker.play(3);

    bool correct = false;
    unsigned long startTime = millis();

    unsigned long currentTime = startTime;
    // check if wired correctly for a certain time

    while((currentTime-startTime) <= timer){ // gets stuck in this loop
      // check for wrong input
      // if key is pressed on keypad
      char key = keypad.getKey();
      if(key){
        return false; // wrong input
      }

      // if mic is above threshold
      analogValue = analogRead(ANALOG_PIN);
      if (analogValue > threshold) {
        return false; // wrong input
      }

      ADS.readADC(0);
      int16_t readingA0 = ADS.getValue();
      Serial.print('A0: ');
      Serial.print(readingA0);
      Serial.print('\n');

      ADS.readADC(1);
      int16_t readingA1 = ADS.getValue();
      Serial.print('A1: ');
      Serial.print(readingA1);
      Serial.print('\n');

      ADS.readADC(2);
      int16_t readingA2 = ADS.getValue();
      Serial.print('A2: ');
      Serial.print(readingA2);
      Serial.print('\n');


      if(WIRE_VOID.isInRange(readingA0) && WIRE_VOID.isInRange(readingA1) && WIRE_VOID.isInRange(readingA2))
      {
        wire_in = 0;
        return true;
      }
      delay(50);
      currentTime = millis();
    }
    return false;
  }


  else if(wire_in == 0){ // if wires are open circuit
    // pick combo to use
    int combo[3] = {-1,-1,-1}; // store current combo
    int combo_idx = -1; // store current combo idx
    speaker.play(3); // wires clip (peter defusing bomb song)

    if(prev_combo_idx == -1){  // if first iteration
      combo_idx = random(0,6);
      }
    else 
    {
        do{
          combo_idx = random(0,6);
          } while(combo_idx == prev_combo_idx);
    }
      for (int i=0;i<3;i++){
        combo[i] = combos[combo_idx][i]; // store current combo
      } 

      prev_combo_idx = combo_idx;
    

    // give combo as instructions on display     display ex: connect the wires:
    Display1.clearDisplay();                     //          Red -> A0
    Display1.setTextSize(0.75);                  //          Green -> A1
    Display1.setTextColor(WHITE, BLACK);         //          Yellow -> A2
    Display1.setCursor(0,0); 
    Display1.println("Connect the wires");

    Display1.print(toString(combo[0]));
    Display1.print(" -> green port");
    Display1.println();

    Display1.print(toString(combo[1]));
    Display1.print(" -> blue port");
    Display1.println();

    Display1.print(toString(combo[2]));
    Display1.print(" -> black port");
    Display1.println();

    Display1.display();

    // add time thing later (&& time < tmax)
    bool correct = false;
    unsigned long startTime = millis();

    unsigned long currentTime = startTime;
    // check if wired correctly for a certain time

    while((currentTime-startTime) <= timer){ // gets stuck in this loop 
      // check for wrong input
      // if key is pressed on keypad
      char key = keypad.getKey();
      if(key){
        return false; // wrong input
      }

      // if mic is above threshold
      analogValue = analogRead(ANALOG_PIN);
      if (analogValue > threshold) {
        return false; // wrong input
      }

      ADS.readADC(0);
      int16_t readingA0 = ADS.getValue();
      Serial.print('A0: ');
      Serial.print(readingA0);
      Serial.print('\n');

      ADS.readADC(1);
      int16_t readingA1 = ADS.getValue();
      Serial.print('A1: ');
      Serial.print(readingA1);
      Serial.print('\n');

      ADS.readADC(2);
      int16_t readingA2 = ADS.getValue();
      Serial.print('A2: ');
      Serial.print(readingA2);
      Serial.print('\n');
      // use the serial readings to debug

      if(combo[0] == 0 && combo[1] == 1 && combo[2] == 2){ // GREEN -> green, BLUE -> blue, BLACK -> black
        if(WIRE_GREEN.isInRange(readingA0) && WIRE_BLUE.isInRange(readingA1) && WIRE_BLACK.isInRange(readingA2)){
          wire_in = 1;
          return true; 
        }
      }
      else if(combo[0] == 0 && combo[1] == 2 && combo[2] == 1){ // GREEN -> green, BLACK -> blue, BLUE -> black
        if(WIRE_GREEN.isInRange(readingA0) && WIRE_BLACK.isInRange(readingA1) && WIRE_BLUE.isInRange(readingA2)){
          wire_in = 1;
          return true;
        }
      }
      else if(combo[0] == 1 && combo[1] == 0 && combo[2] == 2){ // BLUE -> green, GREEN -> blue, BLACK -> black
        if(WIRE_BLUE.isInRange(readingA0) && WIRE_GREEN.isInRange(readingA1) && WIRE_BLACK.isInRange(readingA2)){
          wire_in = 1;
          return true;
        }
      }
      else if(combo[0] == 1 && combo[1] == 2 && combo[2] == 0){ // BLUE -> green, BLACK -> blue, GREEN -> black
        if(WIRE_BLUE.isInRange(readingA0) && WIRE_BLACK.isInRange(readingA1) && WIRE_GREEN.isInRange(readingA2)){
          wire_in = 1;
          return true;
        }
      }
      else if(combo[0] == 2 && combo[1] == 0 && combo[2] == 1){ // BLACK -> green, GREEN -> blue, BLUE -> black
        if(WIRE_BLACK.isInRange(readingA0) && WIRE_GREEN.isInRange(readingA1) && WIRE_BLUE.isInRange(readingA2)){
          wire_in = 1;
          return true;
        }
      }
      else{ // {2,1,0}: BLACK -> green, BLUE -> blue, GREEN -> black
        if(WIRE_BLACK.isInRange(readingA0) && WIRE_BLUE.isInRange(readingA1) && WIRE_GREEN.isInRange(readingA2)){
          wire_in = 1;
          return true;
        }
      }
      delay(50);
      currentTime = millis();
    }
    return false;
  }

  return false;
}




