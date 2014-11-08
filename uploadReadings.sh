# script which builds up curl and send it to server 


cd readings/

url="http://weather.cs.nuim.ie/input.php"

for file in `ls *.txt`
 do
   while IFS='\n' read line
   do 
    result_code=`curl -s -D- --data "$line" $url`
   done < "$file"	
   if grep -q "200" <<< "$result_code"
   then
    #delete the file
	rm $file
	echo "Upload successful, file $file deleted"
   fi		
   
 done
