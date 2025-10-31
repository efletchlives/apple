// HW-484 Sound Sensor Test for ESP32-S3

// Pin definitions
#define ANALOG_PIN 1    // AO - Analog output (ADC1_CH0)
#define DIGITAL_PIN 3   // DO - Digital output

// Variables
int analogValue = 0;
int digitalValue = 0;
int threshold = 200;  // Adjust based on your readings (0-4095 for ESP32)

void setup() {
  Serial.begin(115200);
  
  // Configure pins
  pinMode(DIGITAL_PIN, INPUT);
  pinMode(ANALOG_PIN, INPUT);
  delay(1000);
}

void loop() {
  // Read analog value (0-4095 on ESP32)
  analogValue = analogRead(ANALOG_PIN);
  
  // Read digital value (HIGH when sound detected)
  digitalValue = digitalRead(DIGITAL_PIN);
  
  // Print values
  Serial.print("Analog: ");
  Serial.print(analogValue);
  Serial.print("\t|\tDigital: ");
  Serial.print(digitalValue);
  
  // Check if sound detected
  if (digitalValue == HIGH) {
    Serial.println("\t<-- SOUND DETECTED (Digital)");
  } else if (analogValue > threshold) {
    Serial.println("\t<-- SOUND DETECTED (Analog)");
  } else {
    Serial.println();
  }
  
  delay(100);  // Update every 100ms
}
