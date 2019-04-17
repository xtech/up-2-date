#!/bin/sh

PORT=12346

DIRECTORY=/opt/self-o-mat
NAME=app
BINARY=self_o_mat
TAR=update.tar

TIMEOUT=60 #seconds

BIN=$DIRECTORY/$BINARY
FIRMWARE_NAME=$DIRECTORY/firmware.hex

update()
{
    echo "Updating with timeout $1"

    # Receive and verify tar
    export LD_LIBRARY_PATH=`pwd` && ./update_server $PORT $1 $TAR $NAME
    if [ ! $? -eq 0 ]
    then
        return 1
    fi

    # Unpack and install
    if [ ! -d $DIRECTORY ]; then
        mkdir -p $DIRECTORY
    fi
    tar xf $NAME.tar.gz -C $DIRECTORY

    # Success, we have a file. Check if we need to update the button board
    if [ -f $FIRMWARE_NAME ]
    then
	echo "We have a firmware."
	avrdude -b38400 -P /dev/ttyS0 -c arduino -p atmega328p -v -U flash:w:$FIRMWARE_NAME:i
    else
	echo "No firmware included."
    fi

    chmod +x $BIN
    rm -f $TAR $NAME.sig $NAME.tar.gz $FIRMWARE_NAME
    echo "Update successful"
}

while [ true ]
do
    while [ ! -f $BIN ]
    do
        echo "Missing binary"
        update $TIMEOUT
    done

    echo "Starting"
    (export HOME=/opt && export LD_LIBRARY_PATH=$DIRECTORY/libs && cd $DIRECTORY && eval ./$BINARY)
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
