Candidate number: 15841


## Compile main.c as follows:

- Linux or Mac OS:

        mpicc -o MPI_main MPI_main.c


## Run the Algorithm as follows:

- Linux or Mac OS:

        mpirun -np [Num. of processes] ./MPI_main [M: Num. of row] [N: Num. of columns] [K1:Kernel1 dimension] [K2:Kernel2 dimension]

____


## Example of a main test:
For Matrix:
1  2  3  4  5
6  7  8  9  10
11 12 13 14 15
16 17 18 19 20
21 22 23 24 25
26 27 28 29 30

Kernel1:
1 0 1
0 1 0
1 0 1

Kernel2:
0 1 
0 1

Output after double convolution:
105 115
155 165
205 215

```
exam2 % mpicc -o MPI_main MPI_main.c
exam2 % mpirun -np 2 ./MPI_main

sequential_output[0]=105.000000 and parallel_output[0]=105.000000
sequential_output[1]=115.000000 and parallel_output[1]=115.000000
sequential_output[2]=155.000000 and parallel_output[2]=155.000000
sequential_output[3]=165.000000 and parallel_output[3]=165.000000
sequential_output[4]=205.000000 and parallel_output[4]=205.000000
sequential_output[5]=215.000000 and parallel_output[5]=215.000000
The serialized and MPI paralelized outputs are equal.
```