#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    const int count = 10000000;     ///< Number of array elements
    const int random_seed = 920214; ///< RNG seed
    const int target = 16;          ///< Number to look for

    int* array = 0;                 ///< The array we need to find the max in
    int  index = -1;                ///< The index of the element we need

    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int*)malloc(count*sizeof(int));
    for(int i=0; i<count; i++) { array[i] = rand(); }

    /* Find the index of the element */
    for(int i=0; i<count; i++)
    {
        if(array[i] == target)
        {
            index = i;
            break;
        }
    }

    printf("Found occurence of %d at index %d;\n", target, index);
    return(0);
}
