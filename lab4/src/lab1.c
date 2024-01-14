#include <bits/types/struct_timeval.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define SCHEDULE_TYPE guided // static or guided
#define CHUNK_SIZE 1000      // default 10 or 1000

long long Time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec*1000 + tv.tv_usec/1000;
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

    struct timeval tv;
    gettimeofday(&tv, NULL);

    long long startTime = Time();

    // omp pragma for initialization of the parallelism
    #pragma omp parallel num_threads(threadNum) shared(array, arraySize) reduction(max: max) default(none)
    {
        // parallelism for the loop
        #pragma omp for schedule(SCHEDULE_TYPE, CHUNK_SIZE)
        for (size_t i = 0; i < arraySize; i++) {
            if (array[i] > max) { 
                max = array[i];
            }
        }
    }

    // print time that the calculations took
    printf("Took: %lld", Time() - startTime);
    return 0;
}
