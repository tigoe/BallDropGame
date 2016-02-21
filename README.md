# Ball Drop Game

### A game to demonstrate TCP sockets

This is a multiplayer game in which players collaborate to keep a ball from hitting the ground. Each player has a paddle, and can bound the ball off her paddle. When the ball bounces off your paddle, you get a point. Only the first bounce counts, though; subsequent bounces don't get you points. But if the ball bounces off another player's paddle then back to yours, you score again. You can keep scoring forever by bouncing the ball back and forth.

Here's a [video of the game](https://vimeo.com/156162582) in action.

The game is a networked application. Players log in by making a TCP socket connection to the IP address of the computer on which the game is running. For example, if the game is running on a computer with the IP address 192.168.0.2, then you'd make a connection to 192.168.0.2 port 8080, and you'd be connected.

The simplest connection you can make to the game is via a command line interface, using telnet, like so:

```
$ telnet 192.168.0.2 8080
```
The game itself, BallDropServer, is written in [Processing](http://www.processing.org), so you'll need to download the [Processing IDE](https://processing.org/download/) in order to run it.

## Clients

The game clients can be written in any programming environment, and for any hardware you wish, as long as your environment can connect to a network and make a TCP socket connection. I've written clients for a few platforms below:
* Processing
* [Arduino Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno) using [WiFi101 shield](https://www.arduino.cc/en/Main/ArduinoWiFiShield101)
* [Arduino Uno](https://www.arduino.cc/en/Main/ArduinoBoardUno) using [Ethernet shield](https://www.arduino.cc/en/Main/ArduinoEthernetShield)
* [Arduino Yún](https://www.arduino.cc/en/Main/ArduinoBoardYun) using the [Bridge library](https://www.arduino.cc/en/Reference/YunBridgeLibrary)
* ESP2866/ATTiny84, programmed with Arduino

## Communication Protocol

When a client connects to the server, the server will send back a greeting and the instructions for controlling a paddle, as follows:

```
hi
l = left, r = right, u = up, d = down
x = exit
1 point for each time the ball hits a new paddle
(minimum 2 players)
```

The client can then send the following commands:
* l - move paddle left
* r - move right
* u - move up
* d - move down
* i - toggle paddle label between name or IP address
* n=XXXXX - set name (n= and n are the terminators. Everything between is the name
* x - exit

## Game Play

The game starts when the first new player logs in. Balls will drop from the top of the screen, and remain in play until they reach the bottom of the screen. To score the most points, bounce the ball off as many paddles as you can.

The goal is to get the highest collective score.

The game ends when five balls have dropped. When the game is over, the group's scores will be displayed.

The game in its current version does not send the player any information about the ball or the state of the other players. In order to play the game, you have to be physically present to view the screen of the computer on which the game is running.
