// Evan Fletcher can confirm that this code is PRISTINE!!!!!
#include <Wire.h>
#include "ADS1X15.h"
#include "Voltage_Value.h"
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
//Adafruit_SSD1306 Display1(OLED_RESET);
//Adafruit_SSD1306 Display2(OLED_RESET);

//ADS1115 ADS(0x48);


RangedInt WIRE_RED(14000, 16000, 14800);   // on breadboard: BLACK
RangedInt WIRE_GREEN(7000, 9500, 8500); // on breadboard: GREEN
RangedInt WIRE_YELLOW(16000, 18000, 16500); // on breadboard: YELLOW
RangedInt expected_value;

// key: Red -> 0, Green -> 1, Yellow -> 2
int combos[6][3] = {{0,1,2},  // [i][1] = Wire # to A0,                   
                    {0,2,1},  // [i][2] = Wire # to A1, 
                    {1,0,2},  // [i][3] = Wire # to A2
                    {1,2,0},
                    {2,0,1},
                    {2,1,0}};

int prev_combo_idx = -1; // store previous combo idx
bool done = false; // for wires display

enum wire_colors {
  red = 1,
  green,
  yellow
};

String toString(int color) {
  switch(color) {
    case 0: return "red";
    case 1: return "green";
    case 2: return "yellow";
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


bool wiresLoop(double timer) {
    // pick combo to use
    int combo[3] = {-1,-1,-1}; // store current combo
    int combo_idx = -1; // store current combo idx
    speaker.play(3); // wires clip (peter defusing bomb song)

    if(prev_combo_idx == -1){  // if first iteration
      combo_idx = random(0,6);
      for (int i=0;i<3;i++){
        combo[i] = combos[combo_idx][i]; // store current combo
      }
    }
    else{  // if any iteration after (skip over the previous combo)
      combo_idx = random(0,6);
      if(prev_combo_idx == combo_idx && prev_combo_idx == 5){
        combo_idx = prev_combo_idx - 1;
      }
      else{
        combo_idx = prev_combo_idx + 1;
      }

      for (int i=0;i<3;i++){
        combo[i] = combos[combo_idx][i]; // store current combo
      } 
    }

    // give combo as instructions on display     display ex: connect the wires:
    Display1.clearDisplay();                     //          Red -> A0
    Display1.setTextSize(0.75);                  //          Green -> A1
    Display1.setTextColor(WHITE, BLACK);         //          Yellow -> A2
    Display1.setCursor(0,0); 
    Display1.println("Connect the wires");

    Display1.print(toString(combo[0]));
    Display1.print(" -> A0");
    Display1.println();

    Display1.print(toString(combo[1]));
    Display1.print(" -> A1");
    Display1.println();

    Display1.print(toString(combo[2]));
    Display1.print(" -> A2");
    Display1.println();

    Display1.display();

    // add time thing later (&& time < tmax)
    bool correct = false;
    unsigned long startTime = millis();

    unsigned long currentTime = startTime;
    // check if wired correctly for a certain time

    while((currentTime-startTime) <= timer){ // gets stuck in this loop
      int readingA0 = ADS.readADC(0);
      Serial.println(readingA0);
      int readingA1 = ADS.readADC(1);
      int readingA2 = ADS.readADC(2);

      if(combo[0] == 0 && combo[1] == 1 && combo[2] == 2){ // RED -> A0, GREEN -> A1, YELLOW -> A2
        if(WIRE_RED.isInRange(readingA0) && WIRE_GREEN.isInRange(readingA1) && WIRE_YELLOW.isInRange(readingA2)){
          return true;
        }
      }
      else if(combo[0] == 0 && combo[1] == 2 && combo[2] == 1){ // RED -> A0, YELLOW -> A1, GREEN -> A2
        if(WIRE_RED.isInRange(readingA0) && WIRE_YELLOW.isInRange(readingA1) && WIRE_GREEN.isInRange(readingA2)){
          return true;
        }
      }
      else if(combo[0] == 1 && combo[1] == 0 && combo[2] == 2){ // GREEN -> A0, RED -> A1, YELLOW -> A2
        if(WIRE_GREEN.isInRange(readingA0) && WIRE_RED.isInRange(readingA1) && WIRE_YELLOW.isInRange(readingA2)){
          return true;
        }
      }
      else if(combo[0] == 1 && combo[1] == 2 && combo[2] == 0){ // GREEN -> A0, YELLOW -> A1, RED -> A2
        if(WIRE_GREEN.isInRange(readingA0) && WIRE_YELLOW.isInRange(readingA1) && WIRE_RED.isInRange(readingA2)){
          return true;
        }
      }
      else if(combo[0] == 2 && combo[1] == 0 && combo[2] == 1){ // YELLOW -> A0, RED -> A1, GREEN -> A2
        if(WIRE_YELLOW.isInRange(readingA0) && WIRE_RED.isInRange(readingA1) && WIRE_GREEN.isInRange(readingA2)){
          return true;
        }
      }
      else{ // {3,2,1}: YELLOW -> A0, GREEN -> A1, RED -> A2
        if(WIRE_YELLOW.isInRange(readingA0) && WIRE_GREEN.isInRange(readingA1) && WIRE_RED.isInRange(readingA2)){
          return true;
        }
      }
      currentTime = millis();
    }
  return false;
}




