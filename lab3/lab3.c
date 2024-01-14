#include <omp.h>
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

double shellsort(int *array, int count, int threadNum) {
    double startTime = omp_get_wtime();
    int i, j, tmp, part;

    for (int gap = count / 2; gap > 0; gap /= 2) {
        #pragma omp parallel for shared(gap, count, array) private(i, j, tmp, part) default(none) num_threads(threadNum)
      for(i = 0; i < gap; i++){
            for(part = i + gap; part < count; part += gap){
                for(j=part; j>i  && array[j-gap] > array[j]; j-=gap){
                    tmp = array[j];
                    array[j] = array[j-gap];
                    array[j-gap] = tmp;
                }
            }
        }
    }
    // get current time
    return omp_get_wtime() - startTime;
}

int main(int argc, char **argv) {
    size_t arraySize = 20000000l;

    // maximal element
    int max = -1;

    // check correct usage
    if (argc != 3) {
        fprintf(stderr, "Usage: %s {THREAD_NUM} {SEED}", argv[0]);
        return 1;
    }

    int threadNum = atoi(argv[1]);  // Thread Amount
    int seed = atoi(argv[2]);       // Seed for initialization of random
    
    // Check for overflow or invalid argument in the first place
    if (threadNum <= 0) {
        fprintf(stderr, "Provide thread number in range: 1 - %d", INT_MAX);
        return 1;
    }

    // Random init
    srand(seed);

    int *array = calloc(arraySize * sizeof(int), 1);

    //array initialization with random values
    for (int i = 0; i < arraySize; i++) {
        array[i] = rand();
    }
    fprintf(stdout, "%g", shellsort(array, arraySize, threadNum));

    return 0;
}
