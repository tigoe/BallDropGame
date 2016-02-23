/*
  Joystick reader
  This program reads a joystick on A0 and A1, with
  the pushbutton on pin 5.
  Written and tested on a MKR1000

  created 20 Feb 2016
  by Tom Igoe
*/

const int connectButton = 5;
int prevButtonState = LOW;
int debounceInterval = 15;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  // initialize the joystick button pin as an input:
  pinMode(connectButton, INPUT_PULLUP);    
}

void loop() {
  // check to see if the pushbutton's pressed:
  boolean buttonPushed = buttonRead(connectButton);

  // read the joystick:
  int xSensor = analogRead(A0);
  int ySensor = analogRead(A1);

  // map x and y readings to a  range from -3 to 3:
  xSensor = map(xSensor, 0, 1023, -3, 3);
  ySensor = map(ySensor, 0, 1023, -3, 3);

  // print the state of the sensors:
  Serial.print(buttonPushed);
  Serial.print("\t");
  Serial.print(xSensor);
  Serial.print("\t");
  Serial.println(ySensor);
}


// this method reads the button to see if it's just changed
// from high to low, and debounces the button:

boolean buttonRead(int thisButton) {
  boolean result = false;
  int currentState = digitalRead(thisButton); // current button state

  if (currentState != prevButtonState         // if the button's changed
      && currentState == LOW) {               // and it's low
    result = true;                            // result is true
  }
  
  delay(debounceInterval);
  // save the current state for next time:
  prevButtonState = currentState;
  return result;
}
