//
// All a horrible mess
//
// Compile using: gcc -std=gnu99 -o test test.c pi_dht_read.c pi_mmio.c common_dht_read.c -lrt
//
// Run using: sudo ./test
//
//


//curl --data "device_id=1&time=1414532294&value=23&sensor_type=T" http://weather.cs.nuim.ie/input.php


#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "test.h"
#include "common_dht_read.h"
#include "pi_dht_read.h"

#define FOLDER_NAME "readings"

#define DEVICE_ID "1"
#define TEMP_TYPE "T"
#define HUMID_TYPE "H"
#define DESTINATION_URL "http://weather.cs.nuim.ie/input.php"

int getTimeStamp()
{
	struct timeval timeVal;
	gettimeofday(&timeVal,NULL);
	printf(", Timestamp: %d\n", timeVal.tv_sec);
	int time = ("%d\n", timeVal.tv_sec);
	return time;
}

//create file and write reading values to it
void writeToFile(int temp, int humidity)
{

	int timeStamp = getTimeStamp();
	int paramsLen = 150; 

	int numOfSensors = 2;
	char sensorType[]={'T','H'};
	char readingsParams[numOfSensors][paramsLen];
	

	//char tempParams [paramsLen];
	sprintf(readingsParams[0], "device_id=%s&time=%d&value=%d&sensor_type=%s\n" , DEVICE_ID, timeStamp, temp, TEMP_TYPE);
	
	printf(readingsParams[0]);
	
	
	//char humidityParams [paramsLen];
	sprintf(readingsParams[1] , "device_id=%s&time=%d&value=%d&sensor_type=%s\n" , DEVICE_ID, timeStamp, humidity, HUMID_TYPE);

	printf(readingsParams[1]);
	
	struct stat st = {0};
	
	if(stat(FOLDER_NAME, &st)== -1)
	{
		mkdir(FOLDER_NAME , 0777);
	}

	char fileName[25];

	sprintf(fileName, "%s/%d.txt", FOLDER_NAME, timeStamp);
	printf("Standard name: %s\n", fileName);
	
	
	int i;
	for(i = 0; i < numOfSensors; i++)
	{
		sprintf(fileName, "%s/%d_%c.txt", FOLDER_NAME, timeStamp, sensorType[i]);
		
		//printf("Name with sensor char: %s\n", fileName);

		FILE *f = fopen(fileName, "w");
		
		fputs(readingsParams[i],f);

		if(f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		}


		// fputs(tempParams, f);
		//fputs(humidityParams, f);

		// fflush(f);
		fclose(f);
	}
}

int main(int argc, char *argv[])
{

	int temp, humid;
	int sensorType=DHT11;
	int pin=4; // BCM(pin 4)=BOARD(pin 7)
	float humidity = 0, temperature = 0;
	int tries=10; // try up to 10 times

	// check if elevated
	//getenv - gets value of an environmental varible
	char *envar = getenv("SUDO_COMMAND");
	if (envar == NULL) {
		printf("Run with SUDO\n");
	}

	else {
		int result = DHT_ERROR_CHECKSUM;
		while ((tries>0)&&(result!=DHT_SUCCESS)) {
			tries--;
			// get the sensor reading
			result = pi_dht_read(sensorType, pin, &humidity, &temperature);
		}
		if (result==DHT_SUCCESS)
		{
			printf("Temp=%6.2f°, humidity=%7.2f%% ", temperature, humidity);
			
			temp = ("%6.2f", temperature);
			humid = ("%7.2f%%", humidity);
			
			writeToFile(temp, humid);
		}     
		else
		printf("FAILURE: unable to read sensor\n");
	}

}
