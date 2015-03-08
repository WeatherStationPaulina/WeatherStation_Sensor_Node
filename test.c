//
// Compile using: gcc -std=gnu99 -o test test.c pi_dht_read.c pi_mmio.c common_dht_read.c -lrt
//
// Run using: sudo ./test
//

//curl --data "sensor_id=1&time=1414532294&value=23" http://weather.cs.nuim.ie/input.php

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

#define SENSOR_ID_T "5"
#define SENSOR_ID_H "6"
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
void writeToFile(int temperature, int humidity)
{
	//to be able to create files with rw permissions for pi,
	//since sudo used to run the ./test files have to be accessed for pi user 
	umask(000);
	
	int timeStamp = getTimeStamp();
	int paramsLen = 150; 
	int numOfSensors = 2;
	char sensorType[]={'T','H'};
	char readingsParams[numOfSensors][paramsLen];	

	sprintf(readingsParams[0], "sensor_id=%s&time=%d&value=%d\n" , SENSOR_ID_T, timeStamp, temperature);	
	//printf(readingsParams[0]);
	
	sprintf(readingsParams[1] , "sensor_id=%s&time=%d&value=%d\n" , SENSOR_ID_H, timeStamp, humidity);
	//printf(readingsParams[1]);
	
	struct stat st = {0};
	
	//check if folder already exists
	if(stat(FOLDER_NAME, &st)== -1)
	{
		mkdir(FOLDER_NAME , 0777);
	}

	//build up appropriate file name to store sensor reading value
	char fileName[25];
	sprintf(fileName, "%s/%d.txt", FOLDER_NAME, timeStamp);
		
	int i;
	for(i = 0; i < numOfSensors; i++)
	{
		//add sensor symbol to the file name
		sprintf(fileName, "%s/%d_%c.txt", FOLDER_NAME, timeStamp, sensorType[i]);		
		//printf("Name with sensor char: %s\n", fileName);

		FILE *f = fopen(fileName, "w");
		
		fputs(readingsParams[i],f);

		if(f == NULL)
		{
			printf("Error opening file!\n");
			exit(1);
		} 
		fclose(f);
	}
}

int main(int argc, char *argv[])
{
	int temp, humid;
	int sensorType=DHT22;
	int pin=4; //rPi pin number
	float humidity = 0, temperature = 0;
	int tries=30; // try up to 30 times

	// check if elevated
	//getenv - gets value of an environmental variable
	char *envar = getenv("SUDO_COMMAND");
	if (envar == NULL) 
	{
		printf("Run with SUDO\n");
	}
	else 
	{
		int result = DHT_ERROR_CHECKSUM;
		while ((tries>0)&&(result!=DHT_SUCCESS)) 
		{
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
