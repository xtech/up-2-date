#!/bin/sh

FILES="dummy/dummy dummy/uselessfile"
NAME="dummy"

# build
g++ dummy/main.cpp -o dummy/dummy

# pack and sign
tar cfz "$NAME.tar" $FILES
gpg --quiet --batch --yes --output "$NAME.sig" --detach-sig "$NAME.tar"
tar cfz update.tar "$NAME.tar" "$NAME.sig"
rm "$NAME.tar" "$NAME.sig"

# send
python3 update_client.py

# remove update file
rm update.tar