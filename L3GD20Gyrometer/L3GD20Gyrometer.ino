/*
    Simplified example for the Adafruit LG3D20 gyrometer
    breakout board (https://www.adafruit.com/products/1032)

    Based on Kevin Townsend's original example for the board.
    modified 22 Feb 2016
    by Tom Igoe
*/
#include <Wire.h>
#include <Adafruit_L3GD20.h>
Adafruit_L3GD20 gyro;

void setup() {
  Serial.begin(9600);

  // initialize the gyro:
  if (!gyro.begin()) {
    Serial.println("unable to initialize the sensor. Check your wiring");
    // if you can't contact the gyro, stop:
    while (true);
  }
}

void loop() {
  // read the sensor and print the values:
  gyro.read();
  Serial.print("X: ");
  Serial.print(gyro.data.x);
  Serial.print("\tY: ");
  Serial.print(gyro.data.y);
  Serial.print("\tZ :");
  Serial.println(gyro.data.z);
  delay(100);
}


