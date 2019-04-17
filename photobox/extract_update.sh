#!/bin/sh

PACKET=$1
TARGET=$2
BINARY=$3

echo "Extracting update from $PACKET to $TARGET with binary name $BINARY"

rm -rf $TARGET

# Unpack and install
if [ ! -d $TARGET ]; then
    mkdir -p $TARGET
fi
tar xf $PACKET -C $TARGET
chmod +x $TARGET/$BINARY
