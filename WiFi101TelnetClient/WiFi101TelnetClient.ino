/*
  Joystick client
  Language:  Arduino

  This program enables an Arduino to control one paddle
  in a networked ball drop game. It uses the WiFi101 module, 
  either on a WiFi101 shield, or o a MRK1000 board.

  created 21 Feb 2016
  by Tom Igoe

*/

#include <SPI.h>
#include <WiFi101.h>

char ssid[] = "ssid";     //  your network SSID (name)
char pass[] = "password"; // your network password
int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;
// change this to the address of your server:
IPAddress server(192, 168, 0, 10);

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 5 seconds before attempting again:
    delay(5000);
  }

  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() {
  // if the client's connected, disconnect:
  if (client.connected()) {
    // read any input from the client, send out the serial port:
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    // read any input from the serial port, send out the client:
    while (Serial.available()) {
      char c = Serial.read();
      client.write(c);
    }
  } // if the client's disconnected:
  else {
    // listen for serial input
    while (Serial.available()) {
      char c = Serial.read();
      // if you get a space, try to connect:
      if (c == ' ') {
        Serial.println("connecting");
        client.connect(server, 8080);
      }
    }
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


