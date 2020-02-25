#!/bin/sh
#Написать программу сравнения и вывода результата двух введенных по запросу файлов.
#Информацию переслать через межпроцессный канал в параллельный процесс, где определить число блоков, занимаемых каждым из введенных файлов.
#Предусмотреть возможность прерывания по сигналу <CTRL>+<C>. 
#Если поступило более 3-х сигналов прерывания, то создать новый каталог в текущем и записать туда сравниваемые файлы.
clear
i=0
filenumber=0
bufer=0
chekByNULL()
{

	local spname=$1	
	
	echo $spname ------------ 
	if [ ! -r $spname ] || [ -z $spname ]
	then
		echo введите файл $filenumber
		read spname
	while [ ! -r $spname ] || [ -z $spname ] 
	do
		echo Введите файл $filenumber, а файл \"$spname\" не подходит ... 
		read spname
	done
	fi	
	bufer=$spname
}


sigintHandler(){
	echo "signal == $((i+1))" 
	i=$((i+1))
	if [ $((i % 3)) -eq 0 ]
	then
		echo URA 
		mkdir test/
		cp $file1 ./test/$file1
		cp $file2 ./test/$file2 
	fi 
}

trap 'sigintHandler' 2





if [ $# -eq 2 ]
then
	filenumber=1
	file1=$1
	chekByNULL $file1
	file1=$bufer

	filenumber=2
	file2=$2
	chekByNULL $file2
	file2=$bufer
else
	filenumber=1
	file1=''
	chekByNULL $file1
	file1=$bufer

	filenumber=2
	file2=''
	chekByNULL $file2
	file2=$bufer
fi

tt=`cmp $file1 $file2 && echo "Files are identical"`
echo $tt | awk {'print "Result is:\n " $0'} && du $file1 $file2
# echo $?
# cmp $file1 $file2 && echo "Files are identical" | awk {'print "Result is " $0'}
 # echo $res
# res=`cmp $file1 $file2 | `
# echo pis $res




while true
do
	echo 'Enter "exit" to stop or Ctrl+C to send files in new dir'
	read cmdd
	if [ "$cmdd" = "exit" ];
	then
		exit 0
	fi
done






# ===========================
# if [ $# -eq 2 ]
# then
# file1=$1
# if [ ! -r $file1 ] || [ -z $file1 ]
# then
# 	echo Введите 1 файл
# 	read file1
# while [ ! -r $file1 ] || [ -z $file1 ] 
# do
# 	echo Введите 1 файл
# 	read file1
# done
# fi

# file2=$2
# if [ ! -r $file2 ] || [ -z $file2 ] 
# 	then
# 	echo Введите 2 файл
# 	read file2
# while [ ! -r $file2 ] || [ -z $file2 ] 
# do
# 	echo Введите 2 файл
# 	read file2
# done
# fi


# else
# 	echo Введите 1 файл
# 	read file1
# while [ ! -r $file1 ] || [ -z $file1 ] 
# do
# 	echo Введите 1 файл
# 	read file1
# done

# 	echo Введите 2 файл
# 	read file2
# while [ ! -r $file2 ] || [ -z $file2 ] 
# do
# 	echo Введите 2 файл
# 	read file2
# done
# fi

# flag=0 
# ========================================

# k=`test -r $file1`

# echo "-----= $k "
# t=" "

# while [ $file1 -eq $t] && [ $file2 -eq $t]
# do
# if [ $file1 -eq $t]
# then 
# 	echo Введите 1 файл
# 	read file1
# fi
# if [ $file2 -eq $t]
# then 
# 	echo Введите 2 файл
# 	read file2
# fi
# done

# _______________________________
# if [ -r $file1 ]
# then
# 	if [ -r $file2 ]
# 	then
# 		echo $file1 and $file2 are ok. We are starting.
# 	else
# 		# echo sorry problem with file $file2
# 		while [ ! -r $file2 ]  
# 		do
# 			echo sorry problem with file $file2
# 			read file2
# 		done
# 	fi
# else
# 	# echo sorry $file1 some problem... 
# 	while [ ! -r $file1 ]
# 	do 		
# 		echo sorry problem with file $file1
# 		read file1
# 	done
# 	if [ -r $file2 ]
# 	then
# 		echo $file1 and $file2 are ok. We are starting.
# 	else
# 		# echo sorry problem with file $file2
# 		while [ ! -r $file2 ]
# 		do
# 			echo sorry problem with file $file1
# 			read file2
# 		done
# 	fi
# fi 
# _______________________________



# a=`cmp $name1 $name2 | tee`
# echo $a
# # echo "Введите имя файла 1 и файла 2 через пробел"






# # while [$file1]
# echo "$file1 xiii $file2"

# read file1 file2



# echo "$file1 xiii $file2"

# flag=1

# t=" "
# x=`test -r $t`
# echo xxxxx $?  and $t

# while read -r f1 f2 extra; do  [ -r "$f1" ] && [ -r "$f2" ] && cmp -b "$f1" "$f2"; done <files.txt


# https://coderoad.ru/23429197/Bash-%D0%BA%D0%B0%D0%BA-%D1%80%D0%B0%D1%81%D0%BF%D0%B5%D1%87%D0%B0%D1%82%D0%B0%D1%82%D1%8C-%D1%87%D1%82%D0%BE-%D1%84%D0%B0%D0%B9%D0%BB%D1%8B-%D0%BE%D0%B4%D0%B8%D0%BD%D0%B0%D0%BA%D0%BE%D0%B2%D1%8B-%D0%BF%D1%80%D0%B8-%D0%B8%D1%81%D0%BF%D0%BE%D0%BB%D1%8C%D0%B7%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D0%B8-cmp
# https://habr.com/en/company/ruvds/blog/327754/
# https://habr.com/en/post/471242/