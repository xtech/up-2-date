import sys
import socket

SERVER = sys.argv[1]
PORT = int(sys.argv[2])
FILE = sys.argv[3]

sock = socket.socket()
sock.connect((SERVER, PORT))

with open(FILE, 'wb') as f:
    chunk = sock.recv(1024)
    while (chunk):
        f.write(chunk)
        chunk = sock.recv(1024)
sock.close()
