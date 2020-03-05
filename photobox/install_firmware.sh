#!/bin/sh

FIRMWARE_NAME=$1

echo "Installing firmware in $FIRMWARE_NAME"

# Success, we have a file. Check if we need to update the button board
if [ -f $FIRMWARE_NAME ]
then
echo "We have a firmware."
sudo avrdude -C ./avrdude_updater.conf -c linuxgpio -p atmega328p -v -U flash:w:$FIRMWARE_NAME:i
else
echo "No firmware included."
fi
