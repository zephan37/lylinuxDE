#!/bin/bash
LocalDir=$(cd "$(dirname $0)";pwd)
while true
do
	config_name=$($LocalDir/polybar_launch $1 $2)
	config_name=$(echo $config_name)
	config_path=$LocalDir"/"$config_name
	
	polybar --config=$config_path example
	sleep 120
done
