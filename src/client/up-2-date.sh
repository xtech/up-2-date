#!/bin/sh

SERVER='127.0.0.1'
PORT=12346
BIN="./dummy"

update()
{
    echo "Updating"
    python3 update_client.py $SERVER $PORT $BIN
    chmod +x $BIN
}

while [ true ]
do
    if [ ! -f $BIN ]
        then echo "Missing binary"; update
    fi

    echo "Starting"
    eval $BIN
    ret=$?

    if [ $ret -eq 1 ]
        then echo "Failed"
    elif [ $ret -eq 255 ]
        then update
    fi
done
