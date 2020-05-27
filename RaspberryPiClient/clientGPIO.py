# =========
# Telnet Client + GPIO Buttons
#
# Raspberry client for the BallDropGame assignment
# Using GPIO buttons to send data
# Using python3
# Understanding Networks Fall 2017 NYU ITP
# C.Valenzuela
# cv965@nyu.edu
# =========

import telnetlib
import signal
import sys
import time
import RPi.GPIO as GPIO

# Game Host
SERVER_IP = "172.22.151.123"
SERVER_PORT = "8080"

# Button Pins
LEFT_PIN = 6
RIGHT_PIN = 13
UP_PIN = 19
DOWN_PIN = 26

# GPIO Configuration
GPIO.setmode(GPIO.BCM)
GPIO.setup(LEFT_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(RIGHT_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(UP_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(DOWN_PIN, GPIO.IN, pull_up_down=GPIO.PUD_UP)
  
def exit_gracefully(signum, frame):
    """Handle program exit and close telnet connection"""
    signal.signal(signal.SIGINT, exit_gracefully)

    print("Closing Connection")
    TN.write(("x\n").encode('ascii'))
    sys.exit(1)

    # restore the exit gracefully handler here    
    signal.signal(signal.SIGINT, exit_gracefully)

if __name__ == "__main__":
    original_sigint = signal.getsignal(signal.SIGINT)
    signal.signal(signal.SIGINT, exit_gracefully)

    # Connect to the server game
    TN = telnetlib.Telnet(SERVER_IP, SERVER_PORT)
    TN.read_until(("(minimum 2 players)").encode('ascii'))
    print("BallGame Server on" +  SERVER_IP + ":" + SERVER_PORT)
    
    # Loop
    while True:
        LEFT = GPIO.input(LEFT_PIN)
        RIGHT = GPIO.input(RIGHT_PIN)
        UP = GPIO.input(UP_PIN)
        DOWN = GPIO.input(DOWN_PIN)

        if LEFT == False:
            print('Left')
            TN.write(("l\n").encode('ascii'))
            time.sleep(0.2)
            
        if RIGHT == False:
            print('Left')
            TN.write(("r\n").encode('ascii'))
            time.sleep(0.2)

        if UP == False:
            print('Left')
            TN.write(("u\n").encode('ascii'))
            time.sleep(0.2)

        if DOWN == False:
            print('Left')
            TN.write(("d\n").encode('ascii'))
            time.sleep(0.2)



    
