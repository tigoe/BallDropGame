/*
  Sonar read

  Reads a MaxSonar LV-EZ0, and maps a 1-meter range
  to a range from -3 to 3

  created 22 Feb 2016
  by Tom Igoe
*/

void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the sensor:
  int sensorReading = analogRead(A0);
  // constrain it to a 20 - 40 range:
  sensorReading = constrain(sensorReading, 20, 40);
  // map it to a -3 to 3 range:
  sensorReading = map(sensorReading, 20, 40, -3, 3);
  // print it:
  Serial.println(sensorReading);
  // sensor has a 50ms refresh rate
  delay(50);
}
