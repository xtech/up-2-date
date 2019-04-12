import sys
import socket

PORT = int(sys.argv[1])
TIMEOUT = int(sys.argv[2])
FILE = sys.argv[3]

# Open socket and listen
sock = socket.socket()
sock.bind(('', PORT))
sock.settimeout(TIMEOUT)
sock.listen(1)

# Connect to client
print('Waiting for client')
try:    
    client, addr = sock.accept()
except:
    print('Connection timeout')
    sock.close()
    sys.exit(1)
print('Connected with ', addr)

# Receive file
with open(FILE, 'wb') as f:
    chunk = client.recv(1024)    
    while (chunk):
        f.write(chunk)
        chunk = client.recv(1024)
print('Received file')

client.close()
sock.close()
sys.exit(0)
