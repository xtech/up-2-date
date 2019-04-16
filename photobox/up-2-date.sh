#!/bin/sh

PORT=12346

DIRECTORY=/home/pi/
NAME=self-o-mat
BINARY=build/self_o_mat
TAR=update.tar

TIMEOUT=60 #seconds

BIN=$DIRECTORY/$NAME/$BINARY

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$DIRECTORY/$NAME/build/libs
export LD_LIBRARY_PATH

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
    (cd $DIRECTORY/$NAME && eval $BINARY)
    ret=$?

    if [ $ret -eq 1 ]
    then 
        echo "Failed"
    elif [ $ret -eq 255 ]
    then
        update
    fi
done
