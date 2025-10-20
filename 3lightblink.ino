void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(10, INPUT);

  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  if(digitalRead(10) == HIGH) {
    digitalWrite(6, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(6, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second

    digitalWrite(7, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(7, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second

    digitalWrite(8, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(8, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second
  }
  else{
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
  }
}