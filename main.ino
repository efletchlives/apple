#include <Wire.h>
#include "ADS1X15.h"
#include "Voltage_Value.h"
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define OLED_RESET 4
Adafruit_SSD1306 Display1(OLED_RESET);
Adafruit_SSD1306 Display2(OLED_RESET);

ADS1115 ADS(0x48);

//Begin keypad initialization
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[rows] = {7,2,3,5};
byte colPins[cols] = {6,8,4};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
int numCount = 0;
// end of keypad initialization

//Digital range of values for each voltage level 
RangedInt WIRE_RED(14000, 15700, 14800);   // on breadboard: BLACK
RangedInt WIRE_GREEN(11500, 12500, 12000); // on breadboard: GREEN
RangedInt WIRE_YELLOW(17000, 18000, 17500); // on breadboard: YELLOW
RangedInt expected_value;

// Track which wires and pins have been used
bool wireUsed[3] = {false, false, false};  // RED, GREEN, YELLOW
bool pinUsed[3] = {false, false, false};   // A0, A1, A2

int currentWire = -1;  // Which wire to place next
int currentPin = -1;   // Which pin to place it in

int counter = 0;

void setup()   {
  Serial.begin(9600);

//Display Code
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Display1.clearDisplay();
  Display1.display();

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Display2.clearDisplay();
  Display2.display();

//Wire code
  Wire.begin();
  ADS.begin();
  ADS.setGain(0);
  ADS.setDataRate(7);
  ADS.setMode(1);
  randomSeed(analogRead(0));
}

void loop()
{
  //pick from the three instructions
  //1 == wires (implemented)
  //2 == keypad (working on it)
  //3 == mic (not done)

  if(counter <= 99){
    
    int instruction_id = random(1,4);
    Serial.print("get new instruction ");
    Serial.print(instruction_id);
    Serial.print(" counter = ");
    Serial.print(counter);
    Serial.print(" ");
  
    if(instruction_id == 1){
      bool wiresdone = wires_code();
    }
    
    else if(instruction_id == 2){
      bool keypaddone = keypad_code(); // this works like a charm!
    }

    else if(instruction_id == 3){
      // mic stuff, add here!
    }
  }

  else if(counter >= 100){
    //YOU WIN
    Display1.clearDisplay();
    Display1.setTextSize(2);
    Display1.setTextColor(WHITE, BLACK);
    Display1.setCursor(0,0);
    Display1.println("YOU");
    Display1.println("WIN!");
    Display1.display();
    
    Display2.clearDisplay();
    Display2.setTextSize(2);
    Display2.setTextColor(WHITE, BLACK);
    Display2.setCursor(0,0);
    Display2.println("Score:");
    Display2.println(counter);
    Display2.display();
    
    while (true) {}  // Halt forever

  }
}



bool wires_code() {
  
    int reading = ADS.readADC(currentPin);

    //Picking the corresponding value to the respective wire
    if(currentWire == 0){
       expected_value = WIRE_YELLOW;
    }
    else if(currentWire == 1){
      expected_value = WIRE_GREEN;
    }
    else {
       expected_value = WIRE_RED;
    }


    if(expected_value.isInRange(reading)){
      Serial.println("HOLY SHIT LIGHT");
      Serial.println("YOU WERE RIGHT");

      if(wireUsed[0] && wireUsed[1] && wireUsed[2]){
        Serial.println("U DONE BOI");
        counter++;
        
        Display2.clearDisplay();
        Display2.setTextSize(1);
        Display2.setTextColor(WHITE, BLACK);
        Display2.setCursor(0, 0);
        Display2.println("score:   ");
        Display2.setTextSize(2);
        Display2.print("   ");
        Display2.print(counter);
        Display2.display();
         
        for(int i = 0 ; i < 3 ; i++){
          wireUsed[i] = false;
          pinUsed[i] = false;
        }
        Serial.println("Start a new round!");
        pickNextWireAndPin();
      }
      else {
        delay(1000);
        pickNextWireAndPin();
      }
    }
    return false;
}

void pickNextWireAndPin()
{
  do{
    currentWire = random(0,3); //Picking one of the random 3 wires
  } 
  while (wireUsed[currentWire]); //Pick a random wire while the wireUsed remains false
  wireUsed[currentWire] = true; //Set thes wire we used to true so we know not to use it again

  do{
    currentPin = random(0,3); //Picking one of the random 3 pins
  }
  while(pinUsed[currentPin]); //Picking a random pin while the pinUsed remains false
  pinUsed[currentPin] = true; //Sets the pin we used to true to we know not to use it again

    Display1.clearDisplay();
    Display1.setTextSize(1);
    Display1.setTextColor(WHITE, BLACK);
    Display1.setCursor(0,0);
    Display1.println("Connect the wires");
    Display1.display();


  if(currentWire == 0){
    Display1.print("Connect the Yellow Wire to pin A");
    Display1.println(currentPin);
    Serial.print("Connect the Yellow Wire to pin A");
    Serial.println(currentPin);
  }
  else if(currentWire == 1){
    Display1.print("Connect the Green Wire to Pin A");
    Display1.println(currentPin);
    Serial.print("Connect the Green Wire to Pin A");
    Serial.println(currentPin);
  }
  else {
    Display1.print("Connect to the Red Wire to Pin A");
    Display1.println(currentPin);
    Serial.print("Connect the Red Wire to Pin A");
    Serial.println(currentPin);
  }
  Display1.display();
}


// ----------------------------------------------------------

// works fine, DON'T TOUCH
bool keypad_code(){
  //Initializing random 4-digit code

  String correctCode = "";
  String num1 = String(random(0,10));
  String num2 = String(random(0,10));
  String num3 = String(random(0,10));
  String num4 = String(random(0,10));
  correctCode = num1 + num2 + num3 + num4;

  String code = "";
  numCount = 0;
  
  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE);
  Display1.setCursor(0,0);
  Display1.println(F("Enter 4-digit code: "));
  Display1.println();
  Display1.println(correctCode);
  Display1.display();
  while(numCount <= 5){
    if(numCount == 4){
          if(code == correctCode){
            Display1.clearDisplay();
            Display1.setCursor(0,0);
            Display1.println(F("Good job!"));
            Display1.display();
            delay(1000);
            counter += 1;
            return true;
          }
          else{
            Display1.clearDisplay();
            Display1.println(F("DIE DIE DIE"));
            Display1.display();
            delay(1000);
            counter = 0;
            return false;
          }
      }

      char key;
      while(true){
        key = keypad.getKey();
        if(key){
          break;
        }
      }

      if (key) { // if a key is pressed
          Serial.print("You pressed: ");
          Serial.println(key);

          // Check if key is numeric (0-9)
          if (key >= '0' && key <= '9') {
          numCount++;
          code += key;
          Serial.print("Number count: ");
          Serial.println(numCount);
          }
      }
  }
}
