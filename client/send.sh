#!/bin/sh

IP=192.168.178.123
PORT=12346
TIMEOUT=60 #seconds
TAR=$1

./update_client $IP $PORT $TIMEOUT $TAR
