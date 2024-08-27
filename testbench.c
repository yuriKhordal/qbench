#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include "qbench.h"

// 4 MB
#define ARR_SIZE (64 * 1024)

int samples[ARR_SIZE];
int arr[3][ARR_SIZE];

int int_comp(const void *v1, const void *v2);

void insertIntoList(int *arr, size_t n, int value);
void insertIntoSortedList(int *arr, size_t n, int value);
size_t search(const int *arr, size_t n, int val);
size_t bin_search(const int *arr, size_t n, int val);

int unsortedCreate(void *data);
int insertCreate(void *data);
int qsortCreate(void *data);

int unsortedSearch(void *data);
int binarySearch(void *data);
int stlBinarySearch(void *data);


int main() {
   srand(time(NULL));
   int err;
   for(int i = 0; i < ARR_SIZE; i++) samples[i] = rand();

   bench_t *bench[2];
   bench[0] = createBenchmark("List create");
   benchSub(bench[0], "Unsorted", &arr[0], NULL, unsortedCreate, NULL);
   benchSub(bench[0], "Insertion Sort", &arr[1], NULL, insertCreate, NULL);
   benchSub(bench[0], "Insert then QuickSort", &arr[2], NULL, qsortCreate, NULL);

   bench[1] = createBenchmark("List Search");
   benchSub(bench[1], "Unsorted", &arr[0], NULL, unsortedSearch, NULL);
   benchSub(bench[1], "Own Binary Search", &arr[1], NULL, binarySearch, NULL);
   benchSub(bench[1], "STL Binary Search", &arr[2], NULL, stlBinarySearch, NULL);

   if ((err = benchRun(bench[0]))) {
      printf("Error running benchmark! returned %d\n", err);
   }

   for (size_t i = 0; i < ARR_SIZE-1; i++) {
      if (arr[1][i] > arr[1][i+1]) printf("ERROR");
   }
   for (size_t i = 0; i < ARR_SIZE-1; i++) {
      if (arr[2][i] > arr[2][i+1]) printf("ERROR");
   }
   

   if ((err = benchRun(bench[1]))) {
      printf("Error running benchmark! returned %d\n", err);
   }

   // for (size_t i = 0; i < ARR_SIZE; i += (ARR_SIZE/100)) {
   //    printf("%d, ", arr[0][i]);
   // }
   // printf("\n\n");
   // for (size_t i = 0; i < ARR_SIZE; i += (ARR_SIZE/100)) {
   //    printf("%d, ", arr[1][i]);
   // }
   // printf("\n\n");
   // for (size_t i = 0; i < ARR_SIZE; i += (ARR_SIZE/100)) {
   //    printf("%d, ", arr[2][i]);
   // }
   // printf("\n\n");

   benchRelease(bench[0]);
   return EXIT_SUCCESS;
}

int unsortedCreate(void *data) {
   int *arr = (int*)data;
   for (size_t i = 0; i < ARR_SIZE; i++) {
      insertIntoList(arr, i, samples[i]);
   }
   return 0;
}

int insertCreate(void *data) {
   int *arr = (int*)data;
   for (size_t i = 0; i < ARR_SIZE; i++) {
      insertIntoSortedList(arr, i, samples[i]);
   }
   return 0;
}

int qsortCreate(void *data) {
   int *arr = (int*)data;
   for (size_t i = 0; i < ARR_SIZE; i++) {
      insertIntoList(arr, i, samples[i]);
   }
   qsort(arr, ARR_SIZE, sizeof(int), int_comp);
   return 0;
}

#define SEARCH_TIMES 10000000
int unsortedSearch(void *data) {
   int *arr = (int*)data;
   for (size_t i = 0; i < SEARCH_TIMES; i++) {
      int val = samples[rand() % ARR_SIZE];
      search(arr, ARR_SIZE, val);
   }
   return 0;
}

int binarySearch(void *data) {
   int *arr = (int*)data;
   for (size_t i = 0; i < SEARCH_TIMES; i++) {
      int val = samples[rand() % ARR_SIZE];
      bin_search(arr, ARR_SIZE, val);
   }
   return 0;
}

int stlBinarySearch(void *data) {
   int *arr = (int*)data;
   for (size_t i = 0; i < SEARCH_TIMES; i++) {
      int val = samples[rand() % ARR_SIZE];
      bsearch(&val, arr, ARR_SIZE, sizeof(int), int_comp);
   }
   return 0;
}



int int_comp(const void *v1, const void *v2) {
   if (v1 == v2) return 0;
   const int *i1 = v1, *i2 = v2;
   if (*i1 > *i2) return 1;
   if (*i1 < *i2) return -1;
   return 0;
}

size_t search(const int *arr, size_t n, int val) {
   for (size_t i = 0; i < n; i++) {
      if (val == arr[i]) return i;
   }
   return n;
}

size_t bin_search(const int *arr, size_t n, int val) {
   size_t start = 0, end = n;

   while (start < end) {
      size_t mid = (start + end) / 2;
      if (arr[mid] > val) end = mid;
      else if (arr[mid] < val) start = mid;
      else return mid;
   }
   return n;
}

void insertIntoList(int *arr, size_t n, int value) {
   arr[n] = value;
}

void insertIntoSortedList(int *arr, size_t n, int value) {
   size_t i;
   for (i = n; i > 0 && arr[i-1] > value; i--) {
      arr[i] = arr[i-1];
   }
   arr[i] = value;
}
