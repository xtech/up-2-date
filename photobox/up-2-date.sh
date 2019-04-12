#!/bin/sh

PORT=12346

DIRECTORY=install_path
NAME=dummy
BINARY=dummy
TAR=update.tar

TIMEOUT=60 #seconds

BIN=$DIRECTORY/$NAME/$BINARY

update()
{
    echo "Updating"

    # Receive tar 
    ./update_server $PORT $TIMEOUT $TAR
    if [ ! $? -eq 0 ] || [ ! -f $TAR ]
    then
        echo "Transmission failed"
        return 1
    fi

    # Verify signature
    tar xf $TAR
    gpg --verify $NAME.sig $NAME.tar
    if [ ! $? -eq 0 ]
    then
        echo "Verification failed"
        return 2
    fi

    # Unpack and install
    if [ ! -d $DIRECTORY ]; then
        mkdir $DIRECTORY
    fi
    tar xf $NAME.tar -C $DIRECTORY
    chmod +x $BIN
    rm $TAR $NAME.sig $NAME.tar
    echo "Update successful"
}

while [ true ]
do
    while [ ! -f $BIN ]
    do 
        echo "Missing binary"
        update
    done

    echo "Starting"
    eval $BIN
    ret=$?

    if [ $ret -eq 1 ]
    then 
        echo "Failed"
    elif [ $ret -eq 255 ]
    then
        update
    fi
done
