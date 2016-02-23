/*
  Joystick client

  This program reads a joystick on A0 and A1, with
  the pushbutton on pin 5. It uses those values to connect 
  to a game server.
  Written and tested on a MKR1000

  created 20 Jun 2012
  modified 20 Feb 2016
  by Tom Igoe
*/

#include <SPI.h>
#include <WiFi101.h>

const char ssid[] = "ssid";       //  your network SSID (name)
const char pass[] = "password";   // your network password 

WiFiClient client;                // instance of the WIFi client library
int status = WL_IDLE_STATUS;      // WiFi status
IPAddress server(10,0,0,1);       // address of the server

const int connectButton = 5;      // the pushbutton for connecting/disconnecting
const int connectionLED = 6;      // this LED indicates whether you're connected

const int sendInterval = 100;     // minimum time between messages to the server
const int debounceInterval = 15;  // used to smooth out pushbutton readings
int prevButtonState = 0;          // previous state of the pushbutton
long lastTimeSent = 0;            // timestamp of the last server message

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  // initialize digital inputs and outputs:
  pinMode(connectionLED, OUTPUT);
  pinMode(connectButton, INPUT_PULLUP);
  while (!Serial); // wait for serial port to connect.

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 3 seconds for connection:
    delay(3000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
  // message to send to the server:
  char message = 0;                 
  // note the current time in milliseconds:
  long now = millis();
  // check to see if the pushbutton's pressed:
  boolean buttonPushed = buttonRead(connectButton);

  // if the button's just pressed:
  if (buttonPushed) {
    // if the client's connected, disconnect:
    if (client.connected()) {
      Serial.println("disconnecting");
      client.print("x");
      client.stop();
    } // if the client's disconnected, try to connect:
    else {
      Serial.println("connecting");
      client.connect(server, 8080);
    }
  }

  // read the joystick:
  int xSensor = analogRead(A0);
  int ySensor = analogRead(A1);

  // map x and y readings to a  range from -3 to 3:
  xSensor = map(xSensor, 0, 1023, -3, 3);
  ySensor = map(ySensor, 0, 1023, -3, 3);

  // set a message to send when the sensors are at their edges:
  switch (xSensor) {
    case -3:  // left
      message = 'l';
      break;
    case 3:  // right
      message = 'r';
      break;
  }

  switch (ySensor) {
    case -3:  // up
      message = 'u';
      break;
    case 3:  // down
      message = 'd';
      break;
  }

  if (client.connected()                     // if the client's connected
      && now - lastTimeSent > sendInterval   // and the send interval has elapased
      && message != 0) {                     // and there's a message to send
    client.print(message);                   // send a message
    Serial.print(message);
    //save this moment as last time you sent a message:
    lastTimeSent = now;
  }

  // set the connection LED based on the connection state:
  digitalWrite(connectionLED, client.connected());

  // if there's incoming data from the client, print it serially:
  if (client.available()) {
    char c = client.read();
    Serial.write(c);
  }
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

  delay(debounceInterval);                    // debounce delay
  prevButtonState = currentState;             // save the button state for next time
  return result;
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
