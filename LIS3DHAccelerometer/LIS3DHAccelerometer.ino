/*
    Simplified example for the Adafruit LIS3DH accelerometer
    breakout board (https://www.adafruit.com/products/2809)

    Note: this code sets the accelerometer to 2G (aka 2x the acceleration
    due to gravity). It reads the accelerometer and maps the result
    to a -3 to 3 range.

    modified 22 Feb 2016
    by Tom Igoe
*/

#include <Wire.h>
#include <Adafruit_LIS3DH.h>

Adafruit_LIS3DH accelerometer = Adafruit_LIS3DH();

void setup() {
  Serial.begin(9600);
  while (!Serial);
  if (! accelerometer.begin(0x18)) {
    Serial.println("Couldn't start. Check wiring.");
    while (true);     // stop here and do nothing
  }
  // set accelerometer range:
  accelerometer.setRange(LIS3DH_RANGE_2_G);
}

void loop() {
  accelerometer.read();      // get X Y and Z data at once

  // convert the X, Y and Z readings to a -3 to 3 range:
  Serial.print(convertReading(accelerometer.x));
  Serial.print("\t");
  Serial.print(convertReading(accelerometer.y));
  Serial.print("\t");
  Serial.println(convertReading(accelerometer.z));
}

float convertReading(int reading) {
  // constrain the reading:
  int result = constrain(reading, -16000, 16000);
  // map it to -3 to 3:
  result = map(result, -16000, 16000, -3, 3);
  return result;
}

