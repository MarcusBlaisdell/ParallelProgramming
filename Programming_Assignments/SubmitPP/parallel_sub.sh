#!/bin/sh

#To run this script, please use "sbatch -N<numberofnodes> <path>/sub.sh"

# setting the max wallclock time after which the job will be killed; 
# note: the format is hr:min:sec (default set here to 10 mins)
#SBATCH --time=00:10:00


#MPI helloworld example - this line is a comment
#"np" stands for number of processes.
#this command will run the job on 8 processes. 
#mpirun -np 16 ~/Assignments/03/main

#add in arguments for sub.sh Seed ,A,B,P,N
#Seed being just some random number
#A B being some positive constants
#P being a big constant like a prime
#N being 16, 16 * 2, etc..
mpirun -np 4 ~/cpts_411/main 1423 2131 5235 10007 32

    
# you can add more lines of commands here, if you wish.