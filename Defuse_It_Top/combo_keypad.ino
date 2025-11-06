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
byte colPins[cols] = {0,36,48};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

String enteredCode = "";



void setupKeypad() {
  Serial.begin(9600);
  Wire.begin();
  Display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);


}

bool keypadLoop(double timer) {

  // random keypad code
  String num1 = String(random(0,9));
  String num2 = String(random(0,9));
  String num3 = String(random(0,9));
  String num4 = String(random(0,9));
  correctCode = num1 + num2 + num3 + num4;
  Serial.println(correctCode);

  Display1.clearDisplay();
  Display1.setTextSize(1.5);
  Display1.setTextColor(SSD1306_WHITE);
  Display1.setCursor(0,0);
  Display1.println(F("Enter 4-digit code: "));
  Display1.print("       ");
  Display1.print(correctCode);
  Display1.display();

  speaker.play(2); // keypad clip (siege typing) (this isn't working properly, it's playing 0005.mp3, not 0002.mp3)


  int numCount = 0;
  String code = "";
  unsigned long startTime = millis();
  unsigned long currentTime = startTime;
  
    while(numCount <= 4 && (currentTime-startTime) <= timer)
    {
      char key = keypad.getKey();
      if(key)
      {
        code += key;
        numCount++;

        Serial.print("Key pressed: ");
        Serial.println(key);
      }
 
      if(numCount == 4){
          if(code == correctCode){
            return true;
          }
          else{
            return false; // idk change to whatever correct and incorrect will be
          }
      }
      currentTime = millis();
    }
  return false;

}
