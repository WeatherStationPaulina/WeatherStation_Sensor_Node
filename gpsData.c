#include <stdio.h>
#include "gpsData.h"

char line [ 128 ];

//gps data values
void findGpgga(char fileName[])
{   
	FILE *file = fopen (fileName, "r");
	char gpsValues[] = "$GPGGA";
	int gpsLinePresent = 1;
	int i;
	if ( file != NULL )
	{
		//char line [ 128 ]; /* or other suitable maximum line size */
		while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
		{         
			i = 0;
			for(i; i < 6; ++i)
			{
				if(gpsValues[i] != line[i])
				{
					gpsLinePresent = 0;
					break;
				}		
			}
			if(gpsLinePresent)
			{
				return;
			}				 
		}
		fclose ( file );
	}  
	else
	{
		perror ( fileName );
	}
}

int main ( void ) 
{
	// how to encode gps data http://www.boondog.com/tutorials/gps/gps.html
	char fileName[] = "gpsReadings.txt"; //file with gps raw data
	char gpsLine[128];
	findGpgga(fileName);

	fputs("GPS DATA LINE: ", stdout);
	fputs(line, stdout);
	char gps [14][12];
	int index = 0;
	char lat[25];
	char lng[25];
	char altitude[25];

	char* buffer;
	buffer = (char *)strtok (line, ",");

	while (buffer) 
	{	
		sprintf(gps[index], "%s" , buffer);		
		printf ("Index: %i - %s\n", index, (char*)gps[index]); // process token
		buffer = (char *)strtok (NULL, ",");
		++index;
		while (buffer && *buffer == '\040')
		{
			buffer++;
		}
	}
	sprintf(lat, "Latitude: %s%s", gps[2], gps[3]);
	sprintf(lng, "Longitude: %s%s", gps[4], gps[5]);
	sprintf(altitude, "Altitude: %s", gps[9]);
	
	printf("%s\n", lat);
	printf("%s\n", lng);
	printf("%s\n", altitude); 
	return 0;   
}