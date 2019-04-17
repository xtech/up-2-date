#!/bin/sh

IP=$1
PORT=12346
TIMEOUT=60 #seconds
TAR=$2

curl -X POST $IP:9080/update
./update_client $IP $PORT $TIMEOUT $TAR
