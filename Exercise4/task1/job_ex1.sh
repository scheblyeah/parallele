#!/bin/bash

# Executes job in the queue "lva.q" unless you have special requirements.
#$ -q lva.q

# Changes to the current working directory before performing any further action
#$ -cwd

# Name of your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N my_test_job

# Redirect output stream to this file.
#$ -o output.dat

# Join the error stream to the output stream.
#$ -j yes

# Parallel environment for using OpenMP, allocates 8 cores on a single node
#$ -pe openmp 8

# Use gcc 8.2.0 as the default gcc

module load gcc/8.2.0

make clean
make

export OMP_NUM_THREADS=4

for i in "threads" "cores" "sockets"
do
    echo "Run with OMP_PLACES= $i ..."
    
    export OMP_PLACES=$i
	echo "proc_bin(master)"
    ./task1_master 10000000
	echo "proc_bin(close)"
	./task1_close 10000000
	echo "proc_bin(spread)"
	./task1_spread 10000000
	
done