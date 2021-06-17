#!/bin/bash

# script file to call the parallel program
# multiple times:

# Run 

rm slurm*

for i in {1..11}
do
  declare -i e
  e=2**i
  declare -i n
  n=128*$e
  #echo $n
  ./runit2 $n
done

