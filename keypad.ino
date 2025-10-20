#include <Keypad.h>

char keys[4][3] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
};

int rowPins[4] = {2,3,4,5};
int colPins[3] = {6,7,8};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, 4, 3);

String enteredCode = "";
const String correctCode = "1234";

void setup() {
  Serial.begin(9600);
  Serial.println("Enter 4-digit code:");
}

void loop() {
    if(numCount == 4){
        if(code == correctCode){
            return 1;
        }
        else{
            return 0; // idk change to whatever correct and incorrect will be
        }
    }
    char key = keypad.getKey();
    String code = ""
    if (key) { // if a key is pressed
        Serial.print("You pressed: ");
        Serial.println(key);

        // Check if key is numeric (0-9)
        if (key >= '0' && key <= '9') {
        numCount++;
        code += key
        Serial.print("Number count: ");
        Serial.println(numCount);
        }

        // Optional: reset counter with '*'
        if (key == '*') {
        numCount = 0;
        Serial.println("Counter reset.");
        }
    }
}