#include <SK6812.h>
int trigPin = 11;    // Trigger for HC-SR04 sensor
int echoPin = 12;    // Echo for HC-SR04 sensor
long duration, cm, inches;

SK6812 LED(13); // Number of LEDs in the strip

unsigned long exitTime = 0; // Variable to store the time when person exits the threshold
const unsigned long cooldownTime = 10000; // Cooldown time in milliseconds (10 seconds)

int activeLEDs = 3; // Number of active LEDs
int currentLED = 0; // Variable to track the current position of the active LEDs

void setup() {
  LED.set_output(4); // Digital Pin 4
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  ultrasonicTask(2); // Perform ultrasonic distance sensing
//  LEDTask(cm);        // Manage LED effects based on distance
  
}

void ultrasonicTask(int mode) {
  static unsigned long ultrasonicTimer = 0;
  if (millis() - ultrasonicTimer >= 100) {
    ultrasonicTimer = millis();

    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    pinMode(echoPin, INPUT);
    duration = pulseIn(echoPin, HIGH);
    cm = (duration / 2) / 29.1;
    if(mode == 2){
        // Check if the person is within the 1-meter threshold
        if (cm <= 100) {
          // Check if cooldown time has passed since person exited the threshold
          if (millis() - exitTime >= cooldownTime) {
            activateLEDs(cm);
          }
        } else {
          // Person is outside the threshold, record exit time and turn off LEDs
          turnOffLEDs();
        }

      
      Serial.print(cm);
      Serial.print("cm");
      Serial.println();
      delay(100);
     }

    if(mode == 1 ){
      activateLEDs(cm);
      Serial.println("ultrasonicTask: " + String(cm));
    }

  }
}


void activateLEDs(int currentDistance) {
  int activeLEDs = 3; // Number of active LEDs
  for (int i = 0; i < 13; i++) {
    if (i >= activeLEDs) {
      LED.set_rgbw(i - activeLEDs, {0, 0, 0, 0});
    }
    int red = (i * 128) / 13;
    RGBW color = {0, red, 255 - red, 0};
    LED.set_rgbw(i, color);
    LED.sync();
    delay(100);
  }
  Serial.println("activateLEDs thinks distance is: " + String(currentDistance));
  exitTime = millis(); // Update exitTime when LEDs are activated
  if (currentDistance <= 100) {
    LED.sync();
    ultrasonicTask(1);
  } else {
    turnOffLEDs();
  }
}


void turnOffLEDs() {
  for (int i = 0; i < 13; i++) {
    LED.set_rgbw(i, {0, 0, 0, 0});
  }
  LED.sync();
  currentLED = 0; // Reset the current LED position
  loop();
}