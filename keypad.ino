#include <Keypad.h>

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

String enteredCode = "";
const String correctCode = "1234";
int numCount = 0;
String code = "";

void setup() {
  Serial.begin(9600);
  Serial.println("Enter 4-digit code:");
}

void loop() {
    if(numCount == 4){
        if(code == correctCode){
          Serial.println("Good job!");
          // return 1;
        }
        else{
          Serial.println("DIE DIE DIE");
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
