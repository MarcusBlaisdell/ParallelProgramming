#!/bin/bash

# script file to call the parallel program
# multiple times:

# Run for nonBlocking:

for i in {1..10}
do
  ./Programming_Project_1 0
done

# Run for Blocking:

for i in {1..10}
do
  ./Programming_Project_1 1
done
