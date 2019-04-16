#!/bin/sh

NAME=self-o-mat
BINARY=self_o_mat
FILES="$NAME/build/$BINARY $NAME/build/libs $NAME/assets $NAME/settings.json"
TAR=update.tar

# build
cd $NAME
if [ ! -d build ]; then
    mkdir build
fi
cd build
cmake ..
make

# get libs
if [ ! -d libs ]; then
    mkdir libs
fi
ldd $BINARY | grep "=> /" | while read a b c d; do cp "$c" libs/; done
cd ../..

# pack and sign
tar cfz $NAME.tar.gz $FILES
gpg --quiet --batch --yes --output $NAME.sig --detach-sig $NAME.tar.gz
tar cf $TAR $NAME.tar.gz $NAME.sig
rm $NAME.tar.gz $NAME.sig