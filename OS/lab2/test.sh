#!/bin/sh
clear
bufer=0

chekByNULL()
{
	echo ow
	local spname=$1	
	
	echo $spname ------------ 
	if [ ! -r $spname ] || [ -z $spname ]
	then
		echo Введите файл туут
		read spname
	while [ ! -r $spname ] || [ -z $spname ] 
	do
		echo Введите файл апапапп $spname
		read spname
	done
	fi	
	bufer=$spname
}

name1="1.txt4"
# read name1
chekByNULL $name1
# echo oooooo $?
