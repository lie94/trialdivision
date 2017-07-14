#!/bin/bash

#Print this to a local directory as

make timetest.out

algorithms=(ST LST PT LPT)
tier=100
repetition=13
tests=10000

dir=~/Documents/kexdata

printtofile=true

#for algorithm in $algorithms do
for algorithm in "${algorithms[@]}"
do
  tiertemp=$tier
  filename=$algorithm.dat
  echo "" > $dir/$filename
  for ((i = 0 ; i < repetition ; i++ )); do
    ./timetest.out $algorithm $tiertemp $((tiertemp + tests)) 1 1 >> $dir/$filename
    tiertemp=$((tiertemp*10))
  done
done
