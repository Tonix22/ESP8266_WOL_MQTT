#!/bin/sh
sudo docker run -t -i --mount src="$(pwd)",target=/home/esp/project,type=bind --privileged -v /dev/bus/usb:/dev/bus/usb tonix22/esp8266-idf