Marcus Blaisdell
Programming Project 1

This code is comprised of two parts. 

Part 1: 
Unix script file: loopRun.sh

This file uses a unix script for loop to run "Programming_Project_1" ten times each for the blocking and nonblocking methods.
This was created to force MPI to go out of scope to start a new session for each iteration.

Part 2:
C code file: Programming_Project_1.c

This file contains the C code to run the MPI communications methods, both blocking and nonblocking. 
I use separate functions to run each method separately. The program accepts one argument to specify 
whether it is to be run for blocking or nonblocking and will only run one method per session.
