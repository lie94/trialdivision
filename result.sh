#!/bin/bash

#Print this to a local directory as

make timetest.out

algorithms=(ST LST PT LPT)
start=0
end=100000000
repetition=1
updates=200
dir=~/Documents/kexdata

printtofile=true

#for algorithm in $algorithms do
for algorithm in "${algorithms[@]}"
do
  if [ "$printtofile" = true ]; then
    filename=$algorithm.dat
    echo "" > $dir/$filename
    ./timetest.out $algorithm $start $end $repetition $updates >> $dir/$filename
  else
    echo "You are not printing the results"
    ./timetest.out $algorithm $start $end 0 0 > /dev/null
  fi
done
