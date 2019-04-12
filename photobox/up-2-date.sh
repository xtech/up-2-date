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

    # Receive and verify tar 
    ./update_server $PORT $TIMEOUT $TAR $NAME
    if [ ! $? -eq 0 ]
    then
        return 1
    fi

    # Unpack and install
    if [ ! -d $DIRECTORY ]; then
        mkdir $DIRECTORY
    fi
    tar xf $NAME.tar.gz -C $DIRECTORY
    chmod +x $BIN
    rm $TAR $NAME.sig $NAME.tar.gz
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
