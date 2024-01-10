#include <bits/types/struct_timeval.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

long long Time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    return tv.tv_sec*1000 + tv.tv_usec/1000;
}

int main(int argc, char** argv)
{
  const int count = 100000000;     ///< Number of array elements
  const int target = 16;          ///< Number to look for

  int  index = -1;                ///< The index of the element we need
  if (argc < 3){
    puts("USAGE ./a.out {THREADS_NUM} {SEED}");
    return -1;
  }
  int threads = atoi(argv[1]);         ///< Number of parallel threads to use
  int random_seed = atoi(argv[2]); ///< RNG seed
  if (threads <= 0 ) {
        puts("Provide valid amount of threads.");
        return 1;
  }

  srand(random_seed);

  int *array = malloc(count*sizeof(int));
  if (!array) {
        puts("Could not allocate memory");
    }
  for(int i=0; i<count; i++){ 
    array[i] = rand(); 
  }

  long long time_start = Time();
   // We can multithread index finding
  #pragma omp parallel num_threads(threads) shared(array, count, target) reduction(max: index) default(none)
  {
    #pragma omp for
    for(int i=0; i<count; i++)
    {   
        if(array[i] == target)
        {
            index = i;
            #pragma omp cancel for
        }
    }
  }
  free(array);
  if(index != -1){
    printf("Found occurence of %d at index %d;\n", target, index);
  }
  fprintf(stderr, "%lld", Time() - time_start);
  return 0;
}
