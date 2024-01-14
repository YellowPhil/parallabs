#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>


void initArray(int *array, int len , int seed) {
    srand(seed);
    for (int i = 0; i < len; i++) {
        array[i] = rand();
    }
}


void shellSort(int *array, int len) {
    for(int gap = len/2; gap > 0; gap/=2){
        for(int i = gap; i < len; i++){
            for(int j=i; j>=gap  && array[j-gap] > array[j]; j-=gap){
                int tmp = array[j];
                array[j] = array[j-gap];
                array[j-gap] = tmp;
            }
        }
    }
}

int main(int argc, char **argv) {
    if(argc != 2) {
        return 1;
    }
    int size, rank;
    double startTime;
    MPI_Status status;
    int arraySize = 1000000;

    long seed = atol(argv[1]);

    int ret = MPI_Init(&argc, &argv);

    int *array = calloc(arraySize, sizeof(int));
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_size(MPI_COMM_WORLD, &rank);
    
    int n = arraySize /size;

    if (rank == 0) {
        initArray(array, arraySize, seed);
        seed += rand();
        startTime = MPI_Wtime();

        for (int i = 0; i < size; i++) {
            MPI_Send(array + n * (i - 1), n , MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        shellSort(array + n * (size - 1), arraySize - n  * (size - 1));

        for (int i = 0; i < size - 1; i++) {
            MPI_Recv(array + n * i, n, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
        }

        shellSort(array, arraySize);

        fprintf(stdout, "%g\n", MPI_Wtime() - startTime);
        free(array);
    } else {
        int *cur = calloc(n, sizeof(int));
        MPI_Recv(cur, n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        shellSort(cur, n);
        MPI_Send(cur, n, MPI_INT, 0, 1, MPI_COMM_WORLD);
    }
}
