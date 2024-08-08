#include <Keyboard.h>

/*
  This sketch reads a joystick, i,e. 2 analog inputs
  and one digital input, and sends the results as
  keystrokes, using the Keyboard library. Only works on boards
  which are HID-capable

  created 8 Aug 2024
  by Tom Igoe

  To connect on a POSIX computer:
  nc <server address> <server port>
  Then let the controller do the rest. 
*/


const int connectButton = 2;  // the pushbutton for connecting/disconnecting
const int connectionLED = 3;  // this LED indicates whether you're connected
const int leftLED = 4;        // this LED indicates that you're moving left
const int rightLED = 5;       // this LED indicates that you're moving right
const int upLED = 6;          // this LED indicates that you're moving uo
const int downLED = 7;        // this LED indicates that you're moving down

const int sendInterval = 50;     // minimum time between messages to the server
const int debounceInterval = 5;  // used to smooth out pushbutton readings

int lastButtonState = HIGH;  // previous state of the pushbutton
long lastTimeSent = 0;       // timestamp of the last server message

void setup() {
  // initialize control over the keyboard:
  Keyboard.begin();
  Serial.begin(9600);  // initialize serial
  // initialize digital inputs and outputs:
  pinMode(connectButton, INPUT_PULLUP);
  pinMode(connectionLED, OUTPUT);
  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  pinMode(upLED, OUTPUT);
  pinMode(downLED, OUTPUT);
}

void loop() {
  // check to see if the pushbutton's pressed:
  int buttonState = digitalRead(connectButton);

  // if the button changes state:
  if (buttonState != lastButtonState) {
    // delay for the debounce interval:
    delay(debounceInterval);
    if (buttonState == LOW) {
      // disconnect:
      Keyboard.print("x");
    }
    // save current button state for comparison next time:
    lastButtonState = buttonState;
  }

  // if the client's connected, and the send interval has elapsed:
  if (millis() - lastTimeSent > sendInterval) {
    // read the joystick and send messages as appropriate:
    int xSensor = analogRead(A0);
    delay(1);
    int ySensor = analogRead(A1);

    // map x and y readings to a 3-point range
    // and subtract 1 to get -1 to 1, with
    // 0 at rest:
    xSensor = map(xSensor, 0, 1023, 0, 3) - 1;
    ySensor = map(ySensor, 0, 1023, 0, 3) - 1;

    switch (xSensor) {
      case -1:  //left
        Keyboard.print("a");
        digitalWrite(leftLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(rightLED, LOW);
        digitalWrite(leftLED, LOW);

        break;
      case 1:  // right
        Keyboard.print("d");
        digitalWrite(rightLED, HIGH);
        break;
    }

    switch (ySensor) {
      case -1:  //up
        Keyboard.print("w");
        digitalWrite(upLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(upLED, LOW);
        digitalWrite(downLED, LOW);
        break;
      case 1:  // down
        Keyboard.print("s");
        digitalWrite(downLED, HIGH);
        break;
    }
    //save this moment as last time you sent a message:
    lastTimeSent = millis();
  }
}