# script which builds up curl and send it to server 


cd readings/

files=(*.txt)
url="http://weather.cs.nuim.ie/input.php"

for file in "${files[@]}"
 do
   while IFS='\n' read line
 do
 
	curl --data "$line" $url

 done < "$file"
done
