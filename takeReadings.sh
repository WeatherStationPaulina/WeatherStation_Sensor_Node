#!/bin/bash

#script which take reading of sensors at every hour, run using crone,
cd /home/pi/dht11

    sudo ./test
    ./uploadReadings.sh

