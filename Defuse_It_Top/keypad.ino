#include <Keypad.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET 4
//Adafruit_SSD1306 Display1(128,64,&Wire,OLED_RESET);

String correctCode;
const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[rows] = {35,21,47,45};
byte colPins[cols] = {0,36,46};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

String enteredCode = "";



void setupKeypad() {
  Serial.begin(9600);
  Wire.begin();
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE);
  Display1.setCursor(10,25);
  Display1.println(F("Enter 4-digit code: "));
  Display1.print("       ");
  Display1.print(correctCode);

  Display1.display();



  Serial.print("Enter 4-digit code ");
  Serial.print(correctCode);
  Serial.print(":\n");
}

bool keypadLoop(double timer) {

  // random keypad code
  String num1 = String(random(0,9));
  String num2 = String(random(0,9));
  String num3 = String(random(0,9));
  String num4 = String(random(0,9));
  correctCode = num1 + num2 + num3 + num4;
  int numCount = 0;
  String code = "";
  unsigned long startTime = millis();
  unsigned long endTime;
  
    while(numCount <= 4 && (endTime-startTime) <= timer)
      if(numCount == 4){
          if(code == correctCode){
            Display1.clearDisplay();
            Display1.println(F("Good job!"));
            Display1.display();
            return false;
          }
          else{
            Display1.clearDisplay();
            Display1.println(F("DIE DIE DIE"));
            Display1.display();
            return true; // idk change to whatever correct and incorrect will be
          }
      }

      char key = keypad.getKey();

      //TEMP FOR DEBUGGING
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
          endTime = millis();
      }
  return false;

}
