#!/bin/sh

IP=192.168.178.123
PORT=12346
TIMEOUT=60 #seconds
TAR=$1

curl -X POST $IP:9080/update
./update_client $IP $PORT $TIMEOUT $TAR
