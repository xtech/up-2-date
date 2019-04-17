#!/bin/sh

PORT=12346

TARGET=/opt/self-o-mat
BINARY=self_o_mat
TIMEOUT=60 #seconds

BIN=$TARGET/$BINARY
FIRMWARE_NAME=$TARGET/firmware.hex

update()
{
    echo "Updating with timeout $1"

    # Receive and verify tar
    export LD_LIBRARY_PATH=`pwd` && ./update_server $PORT $1 $TARGET
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
        update $TIMEOUT
    done

    echo "Starting"
    (export HOME=/opt && export LD_LIBRARY_PATH=$TARGET/libs && cd $TARGET && eval ./$BINARY)
    ret=$?

    if [ $ret -eq 66 ]
    then
        echo "Updating"
	update $TIMEOUT
    elif [ $ret -eq 0 ]
    then
	echo "Exited normally - no update"
    else
	echo "Exited with code $ret - updating with shorter timeout"
	update 5
    fi
done
