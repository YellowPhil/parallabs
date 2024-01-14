#include <stdio.h>
#include <unistd.h>
#include <omp.h>

omp_lock_t lock;

int main(){
    size_t result = 0;
    int tmp = 0;

    #pragma omp parallel for private(n) shared(g) num_threads(5000)
    for (size_t i = 0; i < 5000; i++) {
        tmp = omp_get_thread_num();
        omp_set_lock(&lock);
        result += tmp;
        omp_unset_lock(&lock);
    }

    omp_destroy_lock(&lock);
    printf("%ld\n", result);

    return 0;
}
