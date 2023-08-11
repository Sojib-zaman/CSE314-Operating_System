#!/bin/bash
# CAUTION ABOUT MANDATORY ARGUMENTS , DOING WITHOUT IT NOW 
rm -r targets
rm -r unzip
mkdir targets
cd targets 

mkdir C 
mkdir Python
mkdir Java 
touch result.csv

echo "Student_id,type,matched,not matched" >> "result.csv"

cd ..
cd submissions
cd ..
mkdir -p unzip 
cd submissions

extract()
{
	cd .. 
	cd unzip 
	
	filename=${1: -11: -4}
	mkdir "$filename"
	file_unzip "$1" "$filename"
	
}

file_unzip()
{
 cd ..
cd submissions
loc="../unzip/$2"
unzip -d "$loc" "$1" 




cd "$loc"
cd ..
 
 
}


find . -maxdepth 1 -type f -name "*.zip" | while read -r file; do
  extract "$file" 
done

cd ..

target="./targets" 
visit()
{
	

	if [ -d "$1" ]
	then
		
		for i in "$1"/*
		do
			
			visit "$i"
		done
	elif [ -f "$1" ] 
	then 
		filename=$1
		ext="${filename##*.}"
		actual_file="${filename#*/}"
		actual_file="${actual_file%%/*}"
		
		case $ext in 
		*c) mkdir "$target/C/$actual_file"
		cp "$1" "$target/C/$actual_file" 
			
		   ;;
		*a) mkdir  "$target/Java/$actual_file"
		cp  $1 "$target/Java/$actual_file"
			;;
		*y) mkdir "$target/Python/$actual_file"
		cp  $1 "$target/Python/$actual_file"
		
		;;
		*);;
		esac
	fi

}

visit unzip

rm -r unzip

pwd
find_test()
{
	cd tests 
	files=(test*.txt)
	
  	
	cd ..
}
find_test
#now testing 

test()
{
	
	
	if [ -d "$1" ] 
	then
		
		if [[ "${1: -1}" =~ [a-zA-Z]  ]]
		then
			ext="${1#*/}"
		fi
		
		for i in "$1"/*
		do
			
			test "$i"
		done
	elif [ -f "$1" ] 
	then 
		
		
		
		location="${1%/*}"
		filename="${1##*/}"
		
		case $ext in 
		C)




		cd "$location" 
		gcc "$filename" -o main.out 
		count=1
		correct=0



		for file in "${files[@]}";
	do
		./main.out < "../../../tests/$file" > "out${count}.txt"
		ans="../../../answers/ans${count}.txt"
	
		if diff -q "out${count}.txt" "$ans" >/dev/null; then
			correct=$(expr $correct + 1)
		fi
		count=$(expr $count + 1)
	done




	echo $location
	csv="../../result.csv"
	echo "${location##*/} , C , $correct , $(expr $count - 1 - $correct)" >> $csv


		cd ../../.. 
		
		
		 ;;
		Java) 
		cd "$location" 
		echo "$filename"
		#javac Main.java
		
		cd ../../..
		
		
		;;
		Python) 


		cd "$location" 
		count=1
		correct=0


		for file in "${files[@]}";
	do
		python3 "$filename"  < "../../../tests/$file" > "out${count}.txt"
		ans="../../../answers/ans${count}.txt"
	
		if diff -q "out${count}.txt" "$ans" >/dev/null; then
			correct=$(expr $correct + 1)
		fi
		count=$(expr $count + 1)
	done




	echo $location
	csv="../../result.csv"
	echo "${location##*/} ,Python , $correct , $(expr $count - 1 - $correct)" >> $csv


		cd ../../.. ;;
		
		*) ;;
		esac
	fi

}
test targets



























# TRASH FILES NOT IGNORED 









