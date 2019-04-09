import socket

FILE = 'update.tar'
BOX = '127.0.0.1'
PORT = 12346

sock = socket.socket()

while True:
    try:
        con = sock.connect((BOX, PORT))
        break
    except:
    	pass
print('Connected to photobox')

with open(FILE, 'rb') as f:
    sock.sendfile(f, 0)
print('Sent file')
sock.close()
