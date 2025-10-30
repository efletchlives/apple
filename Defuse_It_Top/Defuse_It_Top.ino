
//ENUM For Each Different State
enum State
{
  s0,
  s1,
  s2,
  s3
};

//Global Variable Initialization
int lives;
int score;
const int buttonPin = 2;
State currentState = s0;
bool actionSuccess;

void setupMain() {
  pinMode(buttonPin, INPUT);
  Serial.begin(9600);


}

bool actionChoice()
{
  //1: keypad
  //2: Wires
  //3: Mic
  int choice;

  choice = random(1,4);

  if(choice == 1)
  {
    keypadSuccess = keypadLoop();
  }
  else if(choice == 2)
  {
    wiresLoop();
  }
  else if(choice == 3)
  {
    //mic loop
  }

  



}

void GameLoop() 
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
      actionChoice();
      
      
      break;
    case s2:  //LOSE STATE
      //call wire loop
      
      break;
    case s3:  //WIN STATE
      //call mic loop
      break;
  }

}

// ----------STATE TRANSITIONS----------
void nextState()
{
  //stays in state 0 until button is pressed
  if(currentState == s0)
  {
    while(digitalRead(buttonPin) == LOW)
    {
      currentState = s0;
    }
    currentState = s1;
  }

  else if(currentState == s1)
  {
    if(lives == 0)
    {

    }
  }
}








































