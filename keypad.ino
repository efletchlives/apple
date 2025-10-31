#include <Keypad.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#define OLED_RESET -1
Adafruit_SSD1306 display1(128,64,&Wire,OLED_RESET);

const byte rows = 4;
const byte cols = 3;
char keys[rows][cols] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

byte rowPins[rows] = {35,21,47,45}; // 20 has issue
byte colPins[cols] = {0,36,48};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

String enteredCode = "";

// random keypad code
String num1 = String(random(0,9));
String num2 = String(random(0,9));
String num3 = String(random(0,9));
String num4 = String(random(0,9));
const String correctCode = num1 + num2 + num3 + num4;
int numCount = 0;
String code = "";

void setup() {
  Serial.begin(115200);
  Wire.begin(8,9); // SDA = gpio 8, SCL = gpio 9
  display1.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  display1.clearDisplay();
  display1.setTextSize(1.5);
  display1.setTextColor(SSD1306_WHITE);
  display1.setCursor(10,25);
  display1.println(F("Enter 4-digit code: "));
  display1.print("       ");
  display1.print(correctCode);

  display1.display();


  Serial.print("Enter 4-digit code ");
  Serial.print(correctCode);
  Serial.print(":\n");
}

void loop() {
    if(numCount == 4){
        if(code == correctCode){
          display1.clearDisplay();
          display1.println(F("Good job!"));
          display1.display();
          Serial.print("Good job!");
          while(true){} // infinite loop
          // return 1;
        }
        else{
          display1.clearDisplay();
          display1.println(F("DIE DIE DIE"));
          display1.display();
          Serial.print("DIE DIE DIE");
          while(true){} // infinite loop
          // return 0; // idk change to whatever correct and incorrect will be
        }
    }

    char key = keypad.getKey();

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
