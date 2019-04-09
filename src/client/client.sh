#!/bin/sh

# build, pack and sign
g++ dummy/main.cpp -o dummy/dummy
tar cfz dummy.tar dummy/dummy dummy/uselessfile
gpg --quiet --batch --yes --output dummy.sig --detach-sig dummy.tar
tar cfz update.tar dummy.tar dummy.sig
rm dummy.tar dummy.sig

# send
python3 update_client.py

# remove update file
rm update.tar