#!/bin/bash

if [[ ! -d "$ARDUINO_DIR" ]]; then
	echo "ARDUINO_DIR env var $ARDUINO_DIR is not a directory; exiting"
	exit 1
fi

if [[ -d "$ARDUINO_DIR/hardware/esp8266com" ]]; then
	echo 'The esp8266 appears to be installed already'
	exit 1
fi

mkdir -p "$ARDUINO_DIR/hardware/esp8266com"
ln -s "$(realpath esp8266-arduino)" "$ARDUINO_DIR/hardware/esp8266com/esp8266"
cd "$ARDUINO_DIR/hardware/esp8266com/esp8266/tools"
python get.py
