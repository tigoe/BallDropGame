# =========
# Telnet Client + OSC Server
#
# Raspberry client for the BallDropGame assignment
# Using data from an iOS gyroscope running the gyrOSC app
# Using python3
# Understanding Networks Fall 2017 NYU ITP
# C.Valenzuela
# cv965@nyu.edu
# =========

import telnetlib
import signal
import sys
import argparse
import time

from pythonosc import dispatcher
from pythonosc import osc_server

# Game Host
SERVER_IP = "172.22.151.123"
SERVER_PORT = "8080"

# Local Host for OSC
LOCAL_IP = "172.16.217.150"
LOCAL_HOST = "5005"

def gyro_results(unused_addr, args, x, y, z):
    """Handle gyroscope x,y,z position"""
    millis = int(str(int(round(time.time() * 1000)))[10:11])

    if millis == 3:
        if x > 0.1:
            print("Right")
            TN.write(("r\n").encode('ascii'))
        elif x < -0.1:
            print("Left")
            TN.write(("l\n").encode('ascii'))
        if y < -0.1:
            print("Up")
            TN.write(("u\n").encode('ascii'))
        elif y > 0.1:
            print("Down")
            TN.write(("d\n").encode('ascii'))
    
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

    # Crete an OSC Server and wait for client to send data
    parser = argparse.ArgumentParser()
    parser.add_argument("--ip", default=LOCAL_IP, help="The ip to listen on")
    parser.add_argument("--port", type=int, default=LOCAL_HOST, help="The port to listen on")
    args = parser.parse_args()

    dispatcher = dispatcher.Dispatcher()
    dispatcher.map("/gyrosc/gyro", gyro_results, "Gyro")
    server = osc_server.ThreadingOSCUDPServer((args.ip, args.port), dispatcher)
    print("OSC Server listening on {}".format(server.server_address))
    server.serve_forever()

    



    
