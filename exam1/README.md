# UiO:IN4200 - Exam 1 - Spring2021

Candidate number: 15841

The exam is here: [exam_tasks.pdf]()

Final report is here: [final_report.pdf]()

____
## Introduction and Structure:

This home exam considers connectivity graphs with pairs of nodes directly connected, building nearest neighbors. Each edge of each connection is unweighted and "two-way," and this data representation technique is relevant in the field of data storage. Moreover, we will study a measure of "similarity" between two directly connected nodes, that is, the Shared Nearest Neighbors (SNNs). Finally, a typical unsupervised machine learning task, so-called "clustering based on SNN", will be analyzed.

Our job here is to develop algorithms to read, create SNN, and efficiently identify clusters. The functions to fulfill these tasks were developed using C programming, using serialized, paralleled, and OpenMP techniques, where the files are stored as follows:

```
├── IN4200_High_Performance_Computing_and_Numerical_Projects
|  └── exam1
|  |  └── data
|  |  |  └── facebook_combined.txt
|  |  |  └── graph_example.txt
|  |  └── src
|  |  |  └── check_node.c
|  |  |  └── create_SNN_graph1.c
|  |  |  └── create_SNN_graph1_omp.c
|  |  |  └── create_SNN_graph2.c
|  |  |  └── create_SNN_graph2_omp.c
|  |  |  └── read_graph_from_file1.c
|  |  |  └── read_graph_from_file2.c
|  |  └── report
|  |  |  └── exam_tasks.pdf
|  |  |  └── final_report.pdf
|  |  └── main.c
|  |  └── main.h
|  |  └── README.md
```
____

## Compile main.c as follows:

- Linux:

        gcc -fopenmp main.c main.h


- Mac OS:
    
        clang -Xpreprocessor -fopenmp main.c main.h -lomp

____

## Run the Algorithm as follows:

- Linux or Mac OS:

        ./a.out [connectivity_graphy_data_path] [number_of_threads]

____

## Example of a main test:

```
% ./a.out data/graph_example.txt 4

Testing read_graph_from_file1.c and create_SNN_graph1.c.....
Reading Connectivity Graph 1...
>> CPU time: 0.000079 s.
Please, type 'y' if you want to print table2D here:y
Printing...
0 1 1 1 0
1 0 1 1 0
1 1 0 1 1
1 1 1 0 1
0 0 1 1 0
Creating SNN graph 1 without OMP...
>> CPU time: 0.000019 s && ELAPSED Time: 0.000021 s.
Creating SNN graph 1 with OMP...
>> CPU time: 0.001021 s && ELAPSED Time: 0.000510 s.
Speedup between SNN_table and SNN_table_omp:
>> SNN_table/SNN_table_omp = 0.041141
Comparing if SNN_table and SNN_table_omp are equals:
>> True
Please, type 'y' if you want to print SNN_table here:y
Printing...
0 2 2 2 0
2 0 2 2 0
2 2 0 3 1
2 2 3 0 1
0 0 1 1 0
Please, type 'y' if you want to print SNN_table_omp here:y
Printing...
0 2 2 2 0
2 0 2 2 0
2 2 0 3 1
2 2 3 0 1
0 0 1 1 0

Testing read_graph_from_file2.c and create_SNN_graph2.c.....
Reading Connectivity Graph 2...
>> CPU time: 0.000229 s.
Creating SNN graph 2 without OMP...
>> CPU time: 0.000008 s && ELAPSED Time: 0.000009 s.
Creating SNN graph 2 with OMP...
>> CPU time: 0.000273 s && ELAPSED Time: 0.000114 s.
Speedup between SNN_val and SNN_val_omp:
>> SNN_val/SNN_val_omp = 0.079498
Comparing if SNN_val and SNN_val_omp are equals:
>> True
Please, type 'y' if you want to print SNN_val here:y
Printing SNN_val...
2 2 2 2 2 2 2 2 3 1 2 2 3 1 1 1
Please, type 'y' if you want to print SNN_val here:y
Printing SNN_val...
2 2 2 2 2 2 2 2 3 1 2 2 3 1 1 1

Testing check_node.c......................................
Please, give the node_id number:4
Now, the tau number:1
>> The Nodes that form a cluster with node_id 4 and tau 1: 0,1,3,2
```
