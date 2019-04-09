#!/bin/sh

ADDRESS="127.0.0.1"
PORT=12346

DIRECTORY="install_path"
NAME="dummy"
BINARY="dummy"

BIN="$DIRECTORY/$NAME/$BINARY"

update()
{
    echo "Updating"
    python3 update_server.py $ADDRESS $PORT "update.tar"
    tar xf update.tar
    gpg --verify "$NAME.sig" "$NAME.tar"
    if [ $? -eq 0 ]
    then
        echo "Verification successful"
        if [ ! -d $DIRECTORY ]; then
            mkdir $DIRECTORY
        fi
        tar xf "$NAME.tar" -C $DIRECTORY
        chmod +x $BIN
        rm "update.tar" "$NAME.sig" "$NAME.tar"
        echo "Update successful"
    else
        echo "Verification failed"
    fi
}

while [ true ]
do
    if [ ! -f $BIN ]
    then 
        echo "Missing binary"
        update
    fi

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
