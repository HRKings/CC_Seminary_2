#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <stdlib.h>

void mergesort(int arr[], int size, int stop);
void Merge(int vec[], int vecSize);
int IsSort(int *array, int size);

void mergesort(int arr[], int size, int stop) {
    int mid;
    if(size > 1) {
        mid = size / 2;
        if ( stop > 1 )
        {
            #pragma omp parallel sections
            {
            #pragma omp section
            mergesort(arr, mid, stop-4);
            #pragma omp section
            mergesort(arr + mid, size - mid, stop-4);
            }
            Merge(arr, size);
        }
        else
        {
            mergesort (arr, mid, stop );
            mergesort (arr+mid, size-mid, stop);
            Merge(arr, size);
        }
    }
}

void Merge(int vec[], int vecSize) {
	int mid;
	int i, j, k;
	int* tmp;

	tmp = (int*) malloc(vecSize * sizeof(int));
	if (!tmp)
		exit(1);

	mid = vecSize / 2;

	i = 0;
	j = mid;
	k = 0;

	while (i < mid && j < vecSize) {
		if (vec[i] < vec[j])
			tmp[k] = vec[i++];
		else
			tmp[k] = vec[j++];
		++k;
	}

	if (i == mid)
		while (j < vecSize)
			tmp[k++] = vec[j++];
	else
		while (i < mid)
			tmp[k++] = vec[i++];

	for (i = 0; i < vecSize; ++i)
		vec[i] = tmp[i];

	free(tmp);
}

int IsSorted(int *array, int size) {
	int i, value = 0;
	for(i = 1; i < size; i++)
		if(array[i-1] > array[i])
			return 0;
	return 1;
}

int main(int argc, char** argv) {
int size = 1000000, i, *array;

	array = malloc(size* sizeof(int));
	srand(time(NULL));
	for (i = 0; i < size; i++)
		array[i] = rand()%size;
	double start, end;
	int MAX_THREADS = 8192;     // Caution!

        omp_set_num_threads(MAX_THREADS);

        for (int nested = 0; nested <= 1; nested++) {  // false or true
            omp_set_nested( nested );
                start = omp_get_wtime();
                mergesort(array, 0, size - 1);
                end = omp_get_wtime();

                printf("Tempo: %.3f threads: %d  nested: %d\n", end - start, MAX_THREADS, nested);
                if(IsSort(array, size) == 1)
                    printf("Resultado: Ordenado\n");
                else
                    printf("Resultado: Desordenado\n");
        }
	return 0;
}