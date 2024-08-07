/*
  Ethernet Joystick client
 Language:  Arduino
 
 This program enables an Arduino to control one paddle 
 in a networked Pong game. Uses an Arduino MKR Ethernet shield
 and the Ethernet library.
 
 created 20 Jun 2012
 modified 7 Aug 2024
 by Tom Igoe
 */

#include <SPI.h>
#include <Ethernet.h>

// enter a MAC address here. To be safe on an institutional network,
// use an address where the first octet ends in 2, 6, A, or E,
// e.g. 0x 02, 0x86, 0x4A or 0xDE
// this will ensure that it's a locally administered, unicast address.
// if you know someone else will be using this sketch on your LAN,
// don't use the default address below:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

const char server[] = "192.168.0.3";
EthernetClient client;

const int connectButton = 2;  // the pushbutton for connecting/disconnecting
const int connectionLED = 3;  // this LED indicates whether you're connected
const int leftLED = 4;        // this LED indicates that you're moving left
const int rightLED = 5;       // this LED indicates that you're moving right
const int upLED = 6;          // this LED indicates that you're moving uo
const int downLED = 7;        // this LED indicates that you're moving down
const int ethConnectLED = 8;  // this LED indicates that there's a good ethernet connection
const int ethSelectPin = 10;  // pin for the Ethernet shield chip select

const int sendInterval = 50;     // minimum time between messages to the server
const int debounceInterval = 5;  // used to smooth out pushbutton readings

int lastButtonState = HIGH;  // previous state of the pushbutton
long lastTimeSent = 0;       // timestamp of the last server message

void setup() {
  //Initialize serial
  Serial.begin(9600);
  // if serial monitor's not open, wait 3 seconds:
  if (!Serial) delay(3000);

  // initialize pin  as ETH shield chip select pin:
  Ethernet.init(ethSelectPin);

  // if the Ethernet shield can't be detected:
  while (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.");
    delay(1000);
  }

  // if the cable's not connected or the port it's plugged into isn't live:
  while (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
    delay(1000);
  }

  // try to connect via DHCP:
  while (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    delay(1000);
  }

  // When you're connected, print out the device's network status:
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // turn on LED to indicate a good link:
  pinMode(ethConnectLED, OUTPUT);
  digitalWrite(ethConnectLED, HIGH);

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