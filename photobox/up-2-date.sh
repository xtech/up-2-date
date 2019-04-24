#!/bin/sh

PORT=12346

TARGET=/opt/self-o-mat
BINARY=self_o_mat
TIMEOUT=60 #seconds

BIN=$TARGET/$BINARY
FIRMWARE_NAME=$TARGET/firmware.hex
FORCE_DIR=/media/usb/update.tar

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

while [ true ]
do
    while [ ! -f $BIN ]
    do
        echo "Missing binary"
        update 600 true
    done

    echo "Starting"
    (export HOME=/opt && export LD_LIBRARY_PATH=$TARGET/libs && cd $TARGET && eval ./$BINARY)
    ret=$?

    if [ $ret -eq 66 ]
    then
        echo "Updating"
	update $TIMEOUT false
    elif [ $ret -eq 0 ]
    then
	echo "Exited normally - no update"
    else
	echo "Exited with code $ret - updating with shorter timeout"
	update 5 false
    fi
done
