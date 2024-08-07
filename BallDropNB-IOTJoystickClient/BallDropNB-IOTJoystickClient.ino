/*
  Joystick client
 Language:  Arduino
 
 This program enables an Arduino to control one paddle 
 in a networked Pong game via a TCP socket. Re-written to work with 
 either MKRNB (MKR1500) 

note: connection depends on your SIM card.
 
 created 20 Jun 2012
 modified 7 Aug 2024
 by Tom Igoe
 */

#include <MKRNB.h>
#include "arduino_secrets.h"

// initialize the library instance
NBClient client;
GPRS gprs;
NB nbAccess;

const char server[] = "10.18.22.23";

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
  //Initialize serial
  Serial.begin(9600);
  // if serial monitor's not open, wait 3 seconds:
  if (!Serial) delay(3000);

  // initialize GSM/GPRS, if not connected:
  // After starting the modem with NB.begin()
  // attach to the GPRS network with the APN, login and password
  // connection state
  boolean connected = false;

  while (!connected) {
    if ((nbAccess.begin(SECRET_PINNUMBER) == NB_READY) && (gprs.attachGPRS() == GPRS_READY)) {
      connected = true;
      Serial.println("connected");
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }

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
      // if the client's not connected, connect:
      if (!client.connected()) {
        Serial.println("connecting");
        client.connect(server, 8080);
      } else {  // else disconnect:
        Serial.println("disconnecting");
        client.print("x");
        client.stop();
      }
    }
    // save current button state for comparison next time:
    lastButtonState = buttonState;
  }

  // if the client's connected, and the send interval has elapased:
  if (client.connected() && (millis() - lastTimeSent > sendInterval)) {
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
        client.print("a");
        Serial.print("a");
        digitalWrite(leftLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(rightLED, LOW);
        digitalWrite(leftLED, LOW);
        break;
      case 1:  // right
        client.print("d");
        Serial.print("d");
        digitalWrite(rightLED, HIGH);
        break;
    }

    switch (ySensor) {
      case -1:  //up
        client.print("w");
        Serial.print("w");
        digitalWrite(upLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(upLED, LOW);
        digitalWrite(downLED, LOW);
        break;
      case 1:  // down
        client.print("s");
        digitalWrite(downLED, HIGH);
        break;
    }
    //save this moment as last time you sent a message:
    lastTimeSent = millis();
  }

  // set the connection LED based on the connection state:
  digitalWrite(connectionLED, client.connected());

  // if there's incoming data from the client, print it serially:
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
}