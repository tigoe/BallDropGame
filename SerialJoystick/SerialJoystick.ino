
/*
  This sketch reads a joystick, i,e. 2 analog inputs 
  and one digital input, and sends the results serially

  created 29 Oct 2015
  updated 5 Sept 2017
  by Tom Igoe

  To connect on a POSIX computer:
   cat <serialport> | nc <server address> <server port>
 */
 
// pin numbers for the joystick:
const int xPin = A0;
const int yPin = A1;
const int buttonPin = 2;

int lastButton = 0;     // state of the button in previous loop
int sendInterval = 80;  // min. interval for serial sending
long lastSend = 0;      // timestamp for sending


void setup()  {
  Serial.begin(9600);               // initialize serial
  pinMode(buttonPin, INPUT_PULLUP); // joystick pushbutton
}

void loop() {

  // read the pushbutton:
  int button = !digitalRead(buttonPin);

  // if the button's changed, and it's pressed,
  // send its state serially:
  if (button != lastButton && button == 1) {
    Serial.print('x');
  }
  // save the current state of the button for next loop:
  lastButton = button;

  // read the joystick inputs:
  char x = joystickRead(xPin);
  char y = joystickRead(yPin);
  
  // since the joystick is read constantly,
  // you don't want to send all the time. Only send
  // if it's off-center, and only send every (sendInterval) ms:
  if (millis() - lastSend > sendInterval) {
    if (x != 0) Serial.print(x);
    if (y != 0) Serial.print(y);
    lastSend = millis();
  }
}

//This function reads the joystick pots:

char joystickRead(int axis) {
  int result = 0;                   // result of the read
  int reading = analogRead(axis);   // read the pot
  if (reading > 600) {              // if the pot is off-center on the high side,
    if (axis == xPin) result = 'r'; // send r if it's the x axis
    if (axis == yPin) result = 'u'; // send u if it's the y axis
  } else if (reading < 300 ) {      // if the pot is off-center on the low side,
    if (axis == xPin) result = 'l'; // send l if it's the x axis
    if (axis == yPin) result = 'd'; // send d if it's the y axis
  }
  delay(1);       // delay so you don't read the other axis too soon
  return result;  // return the result
}
