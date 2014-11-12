#!/bin/bash

#script which take reading of sensors every x hours,
cd /home/pi/dht11

while true
  do
    sudo ./test
    ./uploadReadings.sh
	
	echo "Sleeping ..."
	sleep 10m
	
  done
done
