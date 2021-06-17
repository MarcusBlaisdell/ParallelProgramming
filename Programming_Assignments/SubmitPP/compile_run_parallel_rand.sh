#compiles parallel random num gen code then runs it 10 times
mpicc -o main main.c -lm;
for n in {1..10}; do sbatch -N1 parallel_sub.sh; done