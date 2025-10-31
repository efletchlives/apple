#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "ADS1X15.h"

#define OLED_RESET 4
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

//Global Variable Initialization
double timer = 10000;
int lives;
int score;
const int buttonPin = 2;
State currentState = s0;
bool actionSuccess;

void setup() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);

  //DISPLAY SETUP
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Display1.clearDisplay();
  Display1.display();

  Display2.begin(SSD1306_SWITCHCAPVCC, 0x3D);
  Display2.clearDisplay();
  Display2.display();

  //WIRE SETUP
  Wire.begin();
  ADS.begin();
  ADS.setGain(0);
  ADS.setDataRate(7);
  ADS.setMode(1);
  randomSeed(analogRead(0));


}

void actionChoice(int timer)
{
  //1: keypad
  //2: Wires
  //3: Mic
  int choice;

  choice = random(1,4);

  if(choice == 1)
  {
    actionSuccess = keypadLoop(timer);
  }
  else if(choice == 2)
  {
    actionSuccess = wiresLoop(timer);
  }
  else if(choice == 3)
  {
    //mic loop
  }
}


void startScreen()
{
  Display1.clearDisplay();
  Display1.setCursor(0,0);
  Display1.println("DEFUSE-IT----PRESS BUTTON TO START");
  Display1.display();
  delay(2000);
}

void winScreen()
{
  Display1.clearDisplay();
  Display1.setCursor(0,0);
  Display1.println("BOMB DEFUSED");
  Display1.setCursor(0,0);
  Display1.println(score);
  Display1.display();
  delay(2000);
}

void failScreen()
{
  Display1.clearDisplay();
  Display1.setCursor(0,0);
  Display1.println("FAILED TO DEFUSE BOMB");
  Display1.setCursor(0,0);
  Display1.println(score);
  Display1.display();
  delay(2000);
}


void loop() 
{
   
  // ----------FSM BEHAVIOR----------
  switch(currentState)
  {
    case s0:  //STARTUP STATE
      lives = 1;
      score = 0;
      //display lobby
      break;
    case s1:  //GAME LOOP STATE
      //call gameloop
      actionChoice(timer);
    
      break;
    case s2:  //LOSE STATE
      failScreen();
      break;
    case s3:  //WIN STATE
      winScreen();
      break;
  }

  timer -= 71.43;
  nextState();

}

// ----------STATE TRANSITIONS----------
void nextState()
{
  //stays in state 0 until button is pressed
  if(currentState == s0) //START UP STATE
  {
    while(digitalRead(buttonPin) == LOW)
    {
      currentState = s0;
    }
    currentState = s1;
  }

  else if(currentState == s1) //GAME LOOP STATE
  {
    //if there are zero lives left, the game is lost
    if(lives == 0)
    {
      currentState = s2;
    }

    //if the action is successfull but the score is less than 99 then we stay in the game loop
    else if((actionSuccess == true) && (score < 99))
    {
      currentState = s1;
    }

    //if the last action was successfull and the score is 99 then the game is won
    else if((actionSuccess == true) && (score == 99))
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











































