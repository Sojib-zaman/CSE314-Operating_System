#!/bin/bash

show_usage()
{
echo "Usage:"
    echo "./organize.sh <submission folder> <target folder> <test folder> <answer folder> [-v] [-noexecute]" 
    echo "-v : verbose"
    echo "-noexecute: do not execute code files"

}


hyp_arg=0

for x in "$@" ; do
    if [[ "$x" = -* ]]; then
	hyp_arg=$((hyp_arg + 1))
    fi
done
	
	
	
if [[ $# -lt 4 ]]; then
    show_usage
    exit 1
elif [[ $# -ge 4 ]]; then
	
	if [ $(($# - $hyp_arg)) != 4 ]; then 
		show_usage
		exit 1
	fi


fi

submissions="$1"
targets="$2"
tests="$3"
answers="$4"
noexecute=false
varbose=false 

for x in "${@:5}" ; do 
	case $x in 
		-v) verbose=true;;
		-noexecute) noexecute=true ;;
		*) echo "Invalid Argument $x" ;;
	esac
done




rm -rf "$targets"
rm -rf unzip
mkdir -p "$targets"
cd "$targets"


mkdir -p C
mkdir -p Python
mkdir -p Java

if [ $noexecute = false ]; then
	touch result.csv

	echo "student_id ,type ,matched ,not_matched " >> "result.csv"
fi

cd ..
cd "$submissions"
cd ..
mkdir -p unzip 
cd "$submissions"

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
cd "$submissions"
loc="../unzip/$2"
unzip -q -d  "$loc" "$1" 




cd "$loc"
cd ..
 
 
}
unset _JAVA_OPTIONS


find . -type f -name "*.zip" | while read -r file; do
  extract "$file" 
done

cd ..
find_test()
{
	cd "$tests" || exit 1
	files=(test*.txt)
	if [ "$verbose" = true ]; then
		echo "Found ${#files[@]} test files"
  	fi
	cd ..
}
find_test
target="./$targets" 




test()
{

	
	if [ "$noexecute" = false ]; then
		if [ "$verbose" = true ]; then 
			if [[ ${1##*/} =~ ^[0-9] && ${1##*/} != *.*  ]];then
				echo "Executing files of ${1##*/}"
			fi
		fi
	 
		
		if [ -d "$1" ] 
		then
			for i in "$1"/*
			do
				test "$i"
			done
		elif [ -f "$1" ] 
		then 
		
		
			location="${1%/*}"
			filename="${1##*/}"
			
			
			ext="${1#*/}"
			ext="${ext#*/}"
			ext="${ext%%/*}"
			
			
		
			
			case $ext in 
			C)
			cd "$location" 
			gcc "$filename" -o main.out 
			count=1
			correct=0
			for file in "${files[@]}";
		do
			./main.out < "../../../$tests/$file" > "out${count}.txt"
			ans="../../../$answers/ans${count}.txt"
		
			if diff -q "out${count}.txt" "$ans" >/dev/null; then
				 correct=$((correct + 1))
			fi
			count=$((count + 1))
		done
			#echo $location
			csv="../../result.csv"
			echo "${location##*/} , C , $correct , $((count - 1 - correct))" >>"$csv"
			cd ../../..  ;;
			Java) 
			
			
			cd "$location" 
			count=1
			correct=0
			for file in "${files[@]}";
		do
			javac Main.java
			java Main < "../../../$tests/$file" > "out${count}.txt"
			ans="../../../$answers/ans${count}.txt"
		
			if diff -q "out${count}.txt" "$ans" >/dev/null; then
				correct=$((correct + 1))
			fi
			count=$((count + 1))
		done
			#echo $location
			csv="../../result.csv"
			echo "${location##*/} ,Java , $correct , $((count - 1 - correct))" >>"$csv"

			cd ../../..
			
			
			;;
			Python) 
			
		

			cd "$location" 
			count=1
			correct=0


			for file in "${files[@]}";
		do
			python3 "$filename"  < "../../../$tests/$file" > "out${count}.txt"
			ans="../../../$answers/ans${count}.txt"
		
			if diff -q "out${count}.txt" "$ans" >/dev/null; then
				correct=$((correct + 1))
			fi
			      count=$((count + 1))
		done




		#echo $location
			csv="../../result.csv"
			echo "${location##*/} ,Python , $correct , $((count - 1 -correct))" >>"$csv"


			cd ../../.. ;;
			
			*) ;;
			esac
		fi
	fi

}
print_org()
{
	if [ "$verbose" = true ]; then
		echo "Oragnizing files of $1"
  	fi
}

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
		*c) mkdir -p "$target/C/$actual_file"
		cp "$1" "$target/C/$actual_file" 
		print_org "$actual_file"
		test "$target/C/$actual_file"
		
			
		   ;;
		*a) mkdir -p "$target/Java/$actual_file"
		cp  "$1" "$target/Java/$actual_file"
		print_org "$actual_file"
		test "$target/Java/$actual_file"
		
		
		;;
		*y) mkdir -p "$target/Python/$actual_file"
		cp  "$1" "$target/Python/$actual_file"
		print_org "$actual_file"
		test "$target/Python/$actual_file"
		;;
		*);;
		esac
	fi

}

visit unzip

rm -rf unzip












