#!/bin/bash

exec_file=matrice

# Parameters
param=
param=-fopenmp

# Compile
mpicc $param MPI/matrice-mpi.c -I headers -o $exec_file

# execute
# mpirun -np 2 ./$exec_file