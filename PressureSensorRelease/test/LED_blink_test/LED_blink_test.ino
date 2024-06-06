const int ledPin = 13; // set LED pin 13

void setup() {
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);
    Serial.println("LED Blink Test");
}

//LED Test, just to make sure the LED isn't dead
void loop() {
    digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);                      // wait for a second

}
