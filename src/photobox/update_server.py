import sys
import socket

HOST = sys.argv[1]
PORT = int(sys.argv[2])
FILE = sys.argv[3]

sock = socket.socket()
sock.bind((HOST, PORT))
sock.listen(5)
print('Waiting for client')

client, addr = sock.accept()
print('Connected with ', addr)
with open(FILE, 'wb') as f:
    chunk = client.recv(1024)
    while (chunk):
        f.write(chunk)
        chunk = client.recv(1024)
client.close()
print('Received file')
