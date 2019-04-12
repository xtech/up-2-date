#!/bin/sh

FILES="dummy/dummy dummy/uselessfile"
NAME=dummy
TAR=update.tar

IP=127.0.0.1
PORT=12346
TIMEOUT=60 #seconds

# build
g++ dummy/main.cpp -o dummy/dummy

# pack and sign
tar cfz $NAME.tar.gz $FILES
gpg --quiet --batch --yes --output $NAME.sig --detach-sig $NAME.tar.gz
tar cf $TAR $NAME.tar.gz $NAME.sig
rm $NAME.tar.gz $NAME.sig

# send
./update_client $IP $PORT $TIMEOUT $TAR

# remove update file
rm $TAR