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

- Mac OS:
    
        clang -Xpreprocessor -fopenmp main.c main.h -lomp

____

## Run the Algorithm as follows:

- Mac OS:

        ./a.out [connectivity_graphy_data_path]

____

## Example of a test:
