import socket

FILE = 'dummy/dummy'
HOST = '127.0.0.1'
PORT = 12346

sock = socket.socket()
sock.bind((HOST, PORT))
sock.listen(5)

while True:
    client, addr = sock.accept()
    print('Connected with', addr)
    with open(FILE, 'rb') as f:
        client.sendfile(f, 0)
    client.close()
