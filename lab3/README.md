# НИЯУ МИФИ. Лабораторная работа №3. Давыдов Михаил, Б21-502. 2023
---

### Используемая система

**Операционная система** - Linux 6.6.7-arch1-1 x86_64

**Модель процессора** - 12th Gen Intel® Core™ i5-12500H

**Оперативная память** - LPDDR4 3733 MT/s 16GB

**Среда разработки** - `Neovim`

**Версия OpenMP** - `201511`

### Анализ алгоритма

### Оценка временной сложности

- Если массив уже отсортирован - `O(n*log(n))` ( Best case )
- Worst case `O(n^2)`


## Анализ алгоритма

#### Блок схема
![BlockScheme](blockscheme.png)

## Значение директив

`#pragma omp parallel for shared(gap, count, array) private(i, j, tmp, part) default(none) num_threads(threads)`

Задается обасть параллельного цикла, с количеством тредов <code>threads</code>. Переменные <code>array</code>, <code>count</code> и <code>gap</code> объявляются общими для всех тредов и непараллельной части алгоритма. Все новые переменные без явного указания класса не разрешены. Переменные <code>i</code>, <code>j</code>, <code>tmp</code> и <code>part</code> объявляется индивидуальной для каждого треда.
Область - <code>цикл for</code>

Эта директива необходима для распараллеливания сортировки элементов массива, которые отстоят друг от друга на расстоянии <code>gap</code>, потому что они не пересекаются с остальными и соответственно уменьшения время всей сортировки.


## Параллельный алгоритм
200 раз генерируется случайный массив из ста тысяч элементов с разным сидом, чтобы усреднить худшие и лучшие случаи.<br>
Всего эксперимент занял два часа <br>
![TimeSpent](timing.png)<br>

### Среднее время

![AvgTime](AVG_time.png)

### Среднее ускорение
![Acceleration](acceleration.png)
### Средняя эффективность

![Efficiency](efficiency.png)

## Заключение
В данной работе я разработал и реализовал параллельный алгоритм сортировки Шелла. Ускорение возможно, потому что во втором вложенном цикле происходят сортировки пузырьком для элементов отстающих друг от друга на фиксированную величину <code>gap</code>, а следовательно все элементы, стоящие на индексах <code>0..gap-1</code>, образуют непересекающиеся множества.

Анализ графиков показал, что:
- Как и ожидалось, после 16 потоков ускорения не происходит.
- 


## Приложение
### Оценка работы последовательной программы производилось при использовании параллельной программы с одним потоком.
### Оценка работы параллельной программы

```c
#include <omp.h>
#include <stdio.h>

double shellsort(int* array, int count, int threads){
    double t1, t2;
    int i, j, tmp, part;
    t1 = omp_get_wtime();
    for(int gap = count/2; gap > 0; gap /= 2){
        #pragma omp parallel for shared(gap, count, array) private(i, j, tmp, part) default(none) num_threads(threads)
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
    t2 = omp_get_wtime();
    return t2 - t1;
}

int main(int argc, char** argv)
{
  const int count = 10000000;     ///< Number of array elements
  const int target = 16;          ///< Number to look for

  int* array = 0;                 ///< The array we need to find the max in
  int  index = -1;                ///< The index of the element we need
  if (argc < 3){
    puts("USAGE ./a.out {THREADS_NUM} {SEED}");
    return -1;
  }
  const int threads = atoi(argv[1]);         ///< Number of parallel threads to use
  const int random_seed = atoi(argv[2]); ///< RNG seed

  /* Initialize the RNG */
  srand(random_seed);

  /* Generate the random array */

  /*
   * We can multithread array filling
   * */

  array = (int*)malloc(count*sizeof(int));
  for(int i=0; i<count; i++){ 
    array[i] = rand(); 
  }
  double t = shellsort(array, count, threads);
  printf("%g", t);

  return 0;
}
```
