import sys
import socket

HOST = sys.argv[1]
PORT = int(sys.argv[2])
FILE = sys.argv[3]
TIMEOUT = int(sys.argv[4])

sock = socket.socket()
sock.bind((HOST, PORT))
sock.settimeout(TIMEOUT)
sock.listen(1)

print('Waiting for client')
try:    
    client, addr = sock.accept()
except:
    print('Connection timeout')
    sys.exit(1)
print('Connected with ', addr)

with open(FILE, 'wb') as f:
    chunk = client.recv(1024)    
    while (chunk):
        f.write(chunk)
        chunk = client.recv(1024)

print('Received file')
client.close()

sys.exit(0)
