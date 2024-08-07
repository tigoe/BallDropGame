/*
  This sketch reads a joystick, i,e. 2 analog inputs
  and one digital input, and sends the results serially

  created 29 Oct 2015
  updated 7 Aug 2024
  by Tom Igoe

  To connect on a POSIX computer:
   cat <serialport> | nc <server address> <server port>
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
      Serial.print("x");
      // send ctrl-C
      Serial.print(0x1F);
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
        Serial.print("a");
        digitalWrite(leftLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(rightLED, LOW);
        digitalWrite(leftLED, LOW);

        break;
      case 1:  // right
        Serial.print("d");
        digitalWrite(rightLED, HIGH);
        break;
    }

    switch (ySensor) {
      case -1:  //up
        Serial.print("w");
        digitalWrite(upLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(upLED, LOW);
        digitalWrite(downLED, LOW);
        break;
      case 1:  // down
        Serial.print("s");
        digitalWrite(downLED, HIGH);
        break;
    }
    //save this moment as last time you sent a message:
    lastTimeSent = millis();
  }

  // set the connection LED based on the connection state:
  // digitalWrite(connectionLED, connected);
}