# WeatherStation_Sensor_Node

The library for decoding sensor data comes from my supervisor Stephen Brown and it conntains following files:
pi_dht_read.h
pi_dht_read.c
pi_mmio.h
pi_mmio.c
common_dht_read.h
common_dht_read.c
test_dht_read.h
They were NOT developed by me.
---------

Content developed by me:
getForecastData.sh
uploadReadings.sh
takeReadings.sh
getSensorValues.h
getSensorValues.c
testGps.h
testGps.c
gpsData.h
gpsData.c
--- 
gpsReadings.txt generated by ./testGps - with presence of GPS reciever connected to the Raspbery Pi
----------

1. In order to achive sensor node functionality, a sensor either DTH11 or DHT22 needs to be connected to the Raspbery Pi - see wiring.pdf

2. The interface to the library getSensorValues.c needs to be compiled using:
gcc -std=gnu99 -o getSensorValues getSensorValues.c pi_dht_read.c pi_mmio.c common_dht_read.c -lrt

Run using:
sudo ./getSensorValues

3. Bash script need to be made executible by runing:
- chmod +x nameOfTheFile
where name of the file is:
-getForecastData.sh
-uploadReadings.sh
-takeReadings.sh

4.The script for gatthering GPS data needs to be compiled using:
gcc -std=gnu99 -o testGps testGps.c 
Run using: 
In order to achive geoLocation data GPS reciever needs to be connected to the Raspberry Pi
Run using:
./testGps

The data is then save in the folder under gpsReadings.txt (available in the folder)

To decode it testGps.c needs to be compiled using:
gcc -std=gnu99 -o gpsData gpsData.c
Run using: ./gpsData
which prints following data to the screen: latitude, longitude and altitude. 

In order to have the readings taken at required frequency - 1h - readings and 3h- forecast it is necessary to set scripts such ./getForecastData and ./takeReadings in crone tab. 
