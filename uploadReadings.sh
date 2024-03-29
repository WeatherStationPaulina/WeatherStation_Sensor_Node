#!/bin/bash

# script which reads all files line by line, 
# builds up curl using data from one file and send it to server 
cd /home/pi/dht11/readings

url="http://weather.cs.nuim.ie/input.php"

# sorting added in order to start uploading data from the oldest reading
for file in `ls *.txt | sort`
 do
   while IFS='\n' read line
   do 
    result_code=`curl -s -D- --data "$line" $url`
	
	#echo curl --data "$line" $url
   done < "$file"	
   # to check what is server result code, 200 == success
   if grep -q "200" <<< "$result_code" 
   then
    # delete the file	
	rm $file
	echo "Upload successful, file $file deleted"	
   fi

   if  grep -q "409" <<< "$result_code" 
   then
	rm $file
	echo "Duplicate entry , file $file deleted"
   fi
   
 done