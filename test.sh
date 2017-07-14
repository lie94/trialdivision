#!/bin/bash
make
start=1
end=20000
repetition=1
for ((i=start; i<=end; i++)); do
  for ((j=1; j<=repetition; j++)); do
    ./main.out $i
  done
done

echo "Done"
