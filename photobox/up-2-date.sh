#!/bin/sh

PORT=12346

DIRECTORY=/opt/self-o-mat
NAME=app
BINARY=self_o_mat
TAR=update.tar

TIMEOUT=60 #seconds

BIN=$DIRECTORY/$BINARY


update()
{
    echo "Updating"

    # Receive and verify tar
    export LD_LIBRARY_PATH=`pwd` && ./update_server $PORT $TIMEOUT $TAR $NAME
    if [ ! $? -eq 0 ]
    then
        return 1
    fi

    # Unpack and install
    if [ ! -d $DIRECTORY ]; then
        mkdir -p $DIRECTORY
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
    (export LD_LIBRARY_PATH=$DIRECTORY/libs && cd $DIRECTORY && eval ./$BINARY)
    ret=$?

    if [ $ret -eq 1 ]
    then
        echo "Failed"
    elif [ $ret -eq 66 ]
    then
        update
    fi
done
