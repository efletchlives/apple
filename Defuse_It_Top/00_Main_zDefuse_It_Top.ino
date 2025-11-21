#warning "Compiling combo_keypad.ino"
#warning "Compiling combo_mic.ino"

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Keypad.h>
#include "ADS1X15.h"
#include "DFRobotDFPlayerMini.h"

// for DFPlayer#warning "Compiling combo_keypad.ino"
#warning "Compiling combo_mic.ino"

#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <Keypad.h>
#include "ADS1X15.h"
#include "DFRobotDFPlayerMini.h"

// for DFPlayer
#define RXD2 18
#define TXD2 17
DFRobotDFPlayerMini speaker;

#define OLED_RESET -1
Adafruit_SSD1306 Display1(OLED_RESET);
Adafruit_SSD1306 Display2(OLED_RESET);
ADS1115 ADS(0x48);

//ENUM For Each Different State
enum State
{
  s0,
  s1,
  s2,
  s3
};

// wires
bool wire_in = 0; // 0 - wires out, 1 - wires in


bool keypadLoop(double timer,bool &wire_in);
bool wiresLoop(double timer,bool &wire_in);
bool micLoop(double timer,bool &wire_in);

//Global Variable Initialization
double timer = 20000; // change to shorter time maybe
int score = 0;
const int buttonPin = 41;
State currentState = s0;
bool actionSuccess = true;
int max_score = 99;

// keypad
String correctCode;
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};
byte rowPins[rows] = {45,19,20,47};
byte colPins[cols] = {48,0,21};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
String enteredCode = "";


// mic
// Pin definitions
#define ANALOG_PIN 2   // AO - Analog output (ADC1_CH0)
#define DIGITAL_PIN 1   // DO - Digital output

// Variables
int analogValue = 0;
int digitalValue = 0;
int threshold = 3500;  // Adjust based on your readings (0-4095 for ESP32)


void setup() {
  pinMode(38, OUTPUT);
  digitalWrite(38, LOW);

  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  //DISPLAY SETUP
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Display1.clearDisplay();
  Display1.display();

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Display2.clearDisplay();
  Display2.display();

  //WIRE SETUP
  Wire.begin(8,9);
  ADS.begin();
  ADS.setGain(0);
  ADS.setDataRate(7);
  ADS.setMode(1);
  randomSeed(esp_random());

  // DFPLAYER SETUP
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  speaker.begin(Serial1);
  speaker.volume(30);


}

void actionChoice(double timer)
{
  //1: keypad
  //2: Wires
  //3: Mic
  int choice;

  choice = random(1,4);
  Serial.print("action choice = ");
  Serial.println(choice);


  if(choice == 1)
  {
    actionSuccess = keypadLoop(timer,wire_in);
  }
  else if(choice == 2)
  {
    actionSuccess = wiresLoop(timer,wire_in);
  }
  else if(choice == 3)
  {
    actionSuccess = micLoop(timer,wire_in);
  }
}

void startScreen()
{
  Display2.clearDisplay();
  Display2.display();
  score = 0;

  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE);
  Display1.setCursor(0,0);
  Display1.println("      DEFUSE-IT");

  Display1.setTextSize(1);
  Display1.println("PRESS BUTTON TO START");
  Display1.display();

  speaker.play(8); // lobby clip
  // delay(10000);
}

void winScreen()
{
  Serial.println("success");
  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE); 
  Display1.setCursor(0,0);
  Display1.println("   BOMB DEFUSED");
  Display1.display();
  speaker.play(1); // win clip
  delay(15000);
}

void failScreen()
{
  Serial.println("failed");
  Display1.clearDisplay();
  Display1.setCursor(0,0);
  Display1.println("FAILED TO DEFUSE BOMB");
  Display1.display();
  speaker.play(3); // lose clip
  delay(15000);
  
}

void displays()
{
  if(actionSuccess == true){ // did task successfully
    Display1.clearDisplay();
    Display1.setCursor(0,0);
    Display1.println("OBJECTIVE SUCCESS");
    Display1.display();
    delay(2000);

    score++;
    Display2.clearDisplay();
    Display2.setTextSize(1.5);
    Display2.setTextColor(SSD1306_WHITE);
    Display2.setCursor(0,0);
    Display2.println("score:");
    Display2.setCursor(10,25);
    Display2.println(score);
    Display2.display();
  }

  else{ // failed to do task
    Display1.clearDisplay();
    Display1.setCursor(0,0);
    Display1.println("OBJECTIVE FAILED");
    Display1.display();
    delay(2000);
  }
}


void loop() 
{

  actionSuccess = true;

  // ----------FSM BEHAVIOR----------
  switch(currentState)
  {
    case s0:  //STARTUP STATE
        Serial.println("at state 0");
        startScreen();
      break;
    
    case s1:  //GAME LOOP STATE
      //call gameloop
      Serial.println("at state 1");
      actionChoice(timer);
      displays();
      timer -= 71.43;
    
      break;
    case s2:  //LOSE STATE
      Serial.println("at state 2");
      failScreen();
      
      break;
    case s3:  //WIN STATE
      Serial.println("at state 3");
      winScreen();
      break;
  }
  nextState();

}




// ----------STATE TRANSITIONS----------
void nextState()
{
  //stays in state 0 until button is pressed
  if (currentState == s0) //STATE UP STATE
  {

    // If button is pressed (active LOW)
    while(digitalRead(buttonPin) != LOW)
    {
      currentState = s1;
    }
  }

  else if(currentState == s1) //GAME LOOP STATE
  {
    //if the action is failed
    if(actionSuccess == false)
    {
      currentState = s2;
    }

    //if the action is successfull but the score is less than 99 then we stay in the game loop
    else if((actionSuccess == true) && (score < max_score))
    {
      currentState = s1;
    }

    //if the last action was successfull and the score is 99 then the game is won
    else if((actionSuccess == true) && (score == max_score))
    {
      currentState = s3;
    }
  }

  else if(currentState == s2) //GAME LOST
  {
    currentState = s0;
  }

  else if(currentState == s3) //GAME WON
  {
    currentState = s0;
  }
}







#define RXD2 18
#define TXD2 17
DFRobotDFPlayerMini speaker;

#define OLED_RESET -1
Adafruit_SSD1306 Display1(OLED_RESET);
Adafruit_SSD1306 Display2(OLED_RESET);
ADS1115 ADS(0x48);

//ENUM For Each Different State
enum State
{
  s0,
  s1,
  s2,
  s3
};

// wires
bool wire_in = 0; // 0 - wires out, 1 - wires in


bool keypadLoop(double timer,bool &wire_in);
bool wiresLoop(double timer,bool &wire_in);
bool micLoop(double timer,bool &wire_in);

//Global Variable Initialization
double timer = 20000; // change to shorter time maybe
int score = 0;
const int buttonPin = 2;
State currentState = s0;
bool actionSuccess = true;
int max_score = 99;

// keypad
String correctCode;
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};
byte rowPins[rows] = {45,19,20,47};
byte colPins[cols] = {48,0,21};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
String enteredCode = "";


// mic
// Pin definitions
#define ANALOG_PIN 2   // AO - Analog output (ADC1_CH0)
#define DIGITAL_PIN 42   // DO - Digital output

// Variables
int analogValue = 0;
int digitalValue = 0;
int threshold = 3500;  // Adjust based on your readings (0-4095 for ESP32)


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(9600);

  //DISPLAY SETUP
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Display1.clearDisplay();
  Display1.display();

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Display2.clearDisplay();
  Display2.display();

  //WIRE SETUP
  Wire.begin(8,9);
  ADS.begin();
  ADS.setGain(0);
  ADS.setDataRate(7);
  ADS.setMode(1);
  randomSeed(esp_random());

  // DFPLAYER SETUP
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2);
  speaker.begin(Serial1);
  speaker.volume(7); // turn up later


}

void actionChoice(double timer)
{
  //1: keypad
  //2: Wires
  //3: Mic
  int choice;

  choice = random(1,4);
  Serial.print("action choice = ");
  Serial.println(choice);


  if(choice == 1)
  {
    actionSuccess = keypadLoop(timer,wire_in);
  }
  else if(choice == 2)
  {
    actionSuccess = wiresLoop(timer,wire_in);
  }
  else if(choice == 3)
  {
    actionSuccess = micLoop(timer,wire_in);
  }
}

void startScreen()
{
  Display2.clearDisplay();
  Display2.display();
  score = 0;

  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE);
  Display1.setCursor(0,0);
  Display1.println("      DEFUSE-IT");

  Display1.setTextSize(1);
  Display1.println("PRESS BUTTON TO START");
  Display1.display();

  speaker.play(5); // lobby clip
  //delay(10000);
}

void winScreen()
{
  Serial.println("success");
  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE); 
  Display1.setCursor(0,0);
  Display1.println("   BOMB DEFUSED");
  Display1.display();
  delay(2000);
}

void failScreen()
{
  Serial.println("failed");
  Display1.clearDisplay();
  Display1.setCursor(0,0);
  Display1.println("FAILED TO DEFUSE BOMB");
  Display1.display();
  speaker.play(4); // lose clip
  delay(10000);
  
}

void displays()
{
  if(actionSuccess == true){ // did task successfully
    Display1.clearDisplay();
    Display1.setCursor(0,0);
    Display1.println("OBJECTIVE SUCCESS");
    Display1.display();
    delay(2000);

    score++;
    Display2.clearDisplay();
    Display2.setTextSize(1.5);
    Display2.setTextColor(SSD1306_WHITE);
    Display2.setCursor(0,0);
    Display2.println("score:");
    Display2.setCursor(10,25);
    Display2.println(score);
    Display2.display();
  }

  else{ // failed to do task
    Display1.clearDisplay();
    Display1.setCursor(0,0);
    Display1.println("OBJECTIVE FAILED");
    Display1.display();
    delay(2000);
  }
}


void loop() 
{

  actionSuccess = true;

  // ----------FSM BEHAVIOR----------
  switch(currentState)
  {
    case s0:  //STARTUP STATE
        Serial.println("at state 0");
        startScreen();
      break;
    
    case s1:  //GAME LOOP STATE
      //call gameloop
      Serial.println("at state 1");
      actionChoice(timer);
      displays();
      timer -= 71.43;
    
      break;
    case s2:  //LOSE STATE
      Serial.println("at state 2");
      failScreen();
      
      break;
    case s3:  //WIN STATE
      Serial.println("at state 3");
      winScreen();
      break;
  }
  nextState();

}




// ----------STATE TRANSITIONS----------
void nextState()
{
  //stays in state 0 until button is pressed
  if (currentState == s0) //STATE UP STATE
  {

    // If button is pressed (active LOW)
    while(digitalRead(buttonPin) != LOW)
    {
      currentState = s0;
    }

    currentState = s1;
  }

  else if(currentState == s1) //GAME LOOP STATE
  {
    //if the action is failed
    if(actionSuccess == false)
    {
      currentState = s2;
    }

    //if the action is successfull but the score is less than 99 then we stay in the game loop
    else if((actionSuccess == true) && (score < max_score))
    {
      currentState = s1;
    }

    //if the last action was successfull and the score is 99 then the game is won
    else if((actionSuccess == true) && (score == max_score))
    {
      currentState = s3;
    }
  }

  else if(currentState == s2) //GAME LOST
  {
    currentState = s0;
  }

  else if(currentState == s3) //GAME WON
  {
    currentState = s0;
  }
}






