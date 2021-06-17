#!/bin/sh

#To run this script, please use "sbatch -N<numberofnodes> <path>/sub.sh"

# setting the max wallclock time after which the job will be killed; 
# note: the format is hr:min:sec (default set here to 10 mins)
#SBATCH --time=00:10:00


#MPI helloworld example - this line is a comment
#"np" stands for number of processes.
#this command will run the job on 8 processes. 
#mpirun -np 8 ~/Assignments/01/Programming_Project_1
#mpirun -np 64 ~/Assignments/02/main 262144
#mpirun -np 64 ~/Assignments/02/main $1
#mpirun -np 32 ~/Assignments/02/main $1
#mpirun -np 16 ~/Assignments/02/main $1
#mpirun -np 8 ~/Assignments/02/main $1
mpirun -np 4 ~/Assignments/02/main $1
#mpirun -np 2 ~/Assignments/02/main $1
#mpirun -np 1 ~/Assignments/02/main $1


# you can add more lines of commands here, if you wish.
