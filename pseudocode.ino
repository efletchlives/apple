// 
int counter = 0;

int keypad(int counter) {
    // output to display to give command 

    // generate a random 4-number code to input via keypad

    // if pass or fail
    // set counter 0 if fail
    return counter;
}

int wires(int counter) {
    // randomize wire combination (store previous command orientation)
    // output to display to give command 

    // wires sequence, checking for correct input

    // if pass or fail
        // set counter 0 if fail
    return counter;

}

int mic(int counter) {
    // output to display to give command 

    // mic sequence, checking for correct word 

    // use speech library to identify if word said is correct
    // if pass or fail
        // set counter 0 if fail
    return counter;
}

void fail() {
    // put up big red X on display
    // maybe have some sad music play
    // gta wasted sfx / explosion 
}

void win() {
    // play fnaf winning music and AI morgan freeman speech about saving the world and you get $25 starbucks giftcard
}


int main() {
    // wait for button press

    int counter = 0;
    // random select between 3 bop it commands: keypad, wires, mic
    // call function for keypad, wires, and mic commands
    counter = keypad(counter);
    counter = wires(counter);
    counter = mic(counter);

    // if counter = 0, then fail (call fail function)
    fail();

    // if counter = 100, then YOU WIN! hooray!
    win();

}

