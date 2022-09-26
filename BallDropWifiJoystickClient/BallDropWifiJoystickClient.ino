/*
  Joystick client
 Language:  Arduino
 
 This program enables an Arduino to control one paddle 
 in a networked Pong game via a TCP socket. Re-written to work with 
 either WiFi101 library (MKR1000) 
 or WiFiNINA library (Nano 33 IoT, MKR1010)
 
 created 20 Jun 2012
 modified 26 Sept 2022
 by Tom Igoe
 */

#include <SPI.h>
// #include <WiFi101.h>      // use this for MKR1000 board
#include <WiFiNINA.h>  // use this for Nano 33 IoT or MKR1010 boards
#include "arduino_secrets.h"

// Initialize the Wifi client library
WiFiClient client;

const char server[] = "10.18.227.179";

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

  // attempt to connect to Wifi network:
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    // Connect to WPA/WPA2 network.
    WiFi.begin(SECRET_SSID, SECRET_PASS);

    // wait 3 seconds for connection:
    delay(3000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();
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
        client.print("l");
        Serial.print("l");
        digitalWrite(leftLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(rightLED, LOW);
        digitalWrite(leftLED, LOW);

        break;
      case 1:  // right
        client.print("r");
        Serial.print("r");
        digitalWrite(rightLED, HIGH);
        break;
    }

    switch (ySensor) {
      case -1:  //up
        client.print("u");
        Serial.print("u");
        digitalWrite(upLED, HIGH);
        break;
      case 0:  // center
        digitalWrite(upLED, LOW);
        digitalWrite(downLED, LOW);
        break;
      case 1:  // down
        client.print("d");
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

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}