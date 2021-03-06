#!/bin/sh

#To run this script, please use "sbatch -N<numberofnodes> <path>/sub.sh"

# setting the max wallclock time after which the job will be killed; 
# note: the format is hr:min:sec (default set here to 10 mins)
#SBATCH --time=00:10:00


#MPI helloworld example - this line is a comment
#"np" stands for number of processes.
#this command will run the job on 8 processes. 
#mpirun -np 16 ~/Assignments/03/main
mpirun -np 4 ~/Assignments/03/main


# you can add more lines of commands here, if you wish.
