#include <Wire.h>
#include <Adafruit_MPRLS.h>

// You dont *need* a reset and EOC pin for most uses, so we set to -1 and don't connect
#define RESET_PIN  -1  // set to any GPIO pin # to hard-reset on begin()
#define EOC_PIN    -1  // set to any GPIO pin to read end-of-conversion by pin
Adafruit_MPRLS mpr = Adafruit_MPRLS(RESET_PIN, EOC_PIN);

const int ledPin = 13; // set LED pin 13
float previousPressurePSI = 0.0; // Store the previous pressure reading for comparison to current pressure
const int triggerPressure = 7; // Pressurized Cabin Pressure [5PSI gradient] [Change accordingly to -5 PSI ambient]
boolean trigger = false; // Trigger to activate the LED
int readFrequency = 10; // Read frequency in ms [depending on how often/fast you need the sensor to read/react to pressure, change accordingly]
const int buttonPin = 5; // The pin that the button is attached to


void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  Serial.println("MPRLS Simple Test");
  if (! mpr.begin()) {
    Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Found MPRLS sensor");
}

void loop() {

  // Get current pressure, read in from sensor and convert to PSI
  float currentPressurehPA = mpr.readPressure();
  float currentPressurePSI = currentPressurehPA/ 68.947572932; // Convert hPA to PSI
  // Get current time, after 0 start
  unsigned long currentTime = millis();

  //activate trigger if we are at sufficient "altitude/pressure" [should have set this parameter to -5 PSI ambient pressure]
  if (currentPressurePSI <= triggerPressure) {
    trigger = true;
    //Serial.print("Trigger Activated! ");

  }
  // Check if the trigger is activated
  if (trigger == true) {
    // Check if the current pressure has dropped to around [-5] PSI [cabin pressure] within [0.01] seconds
    if (currentPressurePSI >= previousPressurePSI + triggerPressure) {
      digitalWrite(ledPin, HIGH); // Turn on LED
      Serial.println("Pressure drop detected! LED turned on! ");
      delay(10000);

      //reset everything
      Serial.print("Reseting LED and Trigger");
      trigger = false;
      digitalWrite(ledPin, LOW);
    }
    else if (currentPressurePSI > triggerPressure) {
      trigger = false; //disarm the trigger if we are declining slowly
      Serial.print("Trigger Deactivated!");

    }
  }

  // Update previous pressure and time
  previousPressurePSI = currentPressurePSI;


  //Print out the current time and pressure in ms and PSI
  Serial.print(currentTime);
  Serial.print(" ms ,");
  Serial.print(currentPressurePSI);
  Serial.print(" PSI ,");
  Serial.println(trigger);


  delay(readFrequency); // Short delay to prevent spamming (adjust as necessary for your application) [should have set parameter above]
}
