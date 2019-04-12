import sys
import socket
import time

IP = sys.argv[1]
PORT = int(sys.argv[2])
TIMEOUT = int(sys.argv[3])
FILE = sys.argv[4]

sock = socket.socket()

# Try to connect once a second until timeout
print('Trying to connect')
stoptime = time.time() + TIMEOUT
while True:
    try:
        con = sock.connect((IP, PORT))
        break
    except:
        if time.time() < stoptime:
    	    time.sleep(1)
        else:
            print('Connection timeout')
            sock.close()
            sys.exit(1)
print('Connected')

with open(FILE, 'rb') as f:
    sock.sendfile(f, 0)
print('Sent file')

sock.close()