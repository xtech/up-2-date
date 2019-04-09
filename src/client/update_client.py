import socket

FILE = 'dummy/dummy'
BOX = '127.0.0.1'
PORT = 12346

sock = socket.socket()
sock.connect((BOX, PORT))
print('Connected to photobox')

with open(FILE, 'rb') as f:
    sock.sendfile(f, 0)
print('Sent file')
sock.close()
