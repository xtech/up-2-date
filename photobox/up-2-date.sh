#!/bin/sh

PORT=12346

TARGET=/opt/self-o-mat
BINARY=self_o_mat
TIMEOUT=60 #seconds

BIN=$TARGET/$BINARY
FIRMWARE_NAME=$TARGET/firmware.hex
FORCE_DIR=/media/usb/selfomat.update

update()
{
    echo "Updating with timeout $1"

    # Receive and verify tar
    export LD_LIBRARY_PATH=`pwd` && ./update_server $PORT $1 $TARGET $2 $FORCE_DIR
    if [ ! $? -eq 0 ]
    then
        return 1
    fi
}

if [ -f $BIN ]
then
    # We have a binary update once on boot with 5 sec TO
    update 5 BOOT
else
    while [ ! -f $BIN ]
    do
        echo "Missing binary"
        update 600 FIRST
    done
fi

while [ true ]
do

    echo "Starting"
    (export HOME=/opt && export LD_LIBRARY_PATH=$TARGET/libs && cd $TARGET && eval ./$BINARY)
    ret=$?

    if [ $ret -eq 66 ]
    then
        echo "Updating"
	update $TIMEOUT NORM
    elif [ $ret -eq 0 ]
    then
	echo "Exited normally - no update"
    else
	echo "Exited with code $ret - updating with shorter timeout"
	update 5 NORM
    fi
done
