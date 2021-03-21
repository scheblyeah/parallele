#!/bin/bash

# Execute job in the queue "std.q" unless you have special requirements.
#$ -q std.q

# The batch system should use the current directory as working directory.
#$ -cwd

# Redirect output stream to this file.
#$ -o output.dat

# Name your job. Unless you use the -o and -e options, output will
# go to a unique file name.ojob_id for each job.
#$ -N erlacher_sheet3

# Join the error stream to the output stream.
#$ -j yes

# Send status information to this email address. 
#$ -M F.Erlacher@student.uibk.ac.at

# Send me an e-mail when the job has finished. 
#$ -m e

# Parallel environment for using OpenMP, allocates 8 cores on a single node
#$ -pe openmp 8

# Specify the amount of virtual memory given to each MPI process
# in the job.
#$ -l h_vmem=2G

module load gcc/8.2.0

make clean

make all

valgrind --tool=cachegrind ./ex2 1

valgrind --tool=cachegrind ./ex2 2

perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores ./ex2 1

perf stat -e instructions,L1-dcache-loads,L1-dcache-load-misses,L1-dcache-stores ./ex2 2

#export FOO=bar