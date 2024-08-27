#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include "qbench.h"

#define ERR_MEM -1
#define BENCH_SUCC 0

void displayTime(const struct timespec *start, const struct timespec *end);

bench_t * createBenchmark(const char *title) {
   bench_t *bench = malloc(sizeof(bench_t));
   if (!bench) return NULL;

   bench->subs_count = 0;
   bench->subs_size = 8;
   bench->subs = malloc(8 * sizeof(struct bench_sub_t));
   if (!bench->subs) {
      free(bench);
      return NULL;
   }

   bench->title = strdup(title);
   if (!bench->title) {
      free(bench->subs);
      free(bench);
      return NULL;
   }

   return bench;
}

int benchSub(bench_t *bench, const char *name, void *data,
bench_func_t init, bench_func_t benchmark, bench_func_t release) {
   if (bench->subs_count == bench->subs_size) {
      bench->subs_size *= 2;
      struct bench_sub_t *temp = realloc(bench->subs,
         sizeof(struct bench_sub_t) * bench->subs_size);
      if (!temp) return ERR_MEM;
      bench->subs = temp;
   }

   bench->subs[bench->subs_count].name = strdup(name);
   if (!bench->subs[bench->subs_count].name) return ERR_MEM;

   bench->subs[bench->subs_count].data = data;
   bench->subs[bench->subs_count].init = init;
   bench->subs[bench->subs_count].bench = benchmark;
   bench->subs[bench->subs_count].release = release;

   bench->subs_count++;

   return BENCH_SUCC;
}

int benchRun(const bench_t *bench) {
   struct timespec real_start, real_end;
   struct timespec cpu_start, cpu_end;
   printf("%s\n", bench->title);

   for (size_t i = 0; i < bench->subs_count; i++) {
      struct bench_sub_t *sub = bench->subs + i;
      int err;
      printf("\tBenchmarking %s:\n", sub->name);
      // Initialize the benchmark:
      if (sub->init && (err = sub->init(sub->data)) < 0) {
         printf("Error initializing benchmark %s.\n", sub->name);
         return err + 10;
      }

      // Time and execute the benchmark:
      clock_gettime(CLOCK_REALTIME, &real_start);
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_start);
      if ((err = sub->bench(sub->data)) < 0) {
         printf("Error benchmarking %s.\n", sub->name);
         return err + 20;
      }
      clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &cpu_end);
      clock_gettime(CLOCK_REALTIME, &real_end);

      // Calculate benchmark timing and display results:
      printf("\t%s - CPU Time: ", sub->name);
      displayTime(&cpu_start, &cpu_end);
      printf(", Real Time: ");
      displayTime(&real_start, &real_end);
      printf("\n");

      // Finilize the benchmark:
      if (sub->release && (err = sub->release(sub->data)) < 0) {
         printf("Error releasing benchmark %s's memory.\n", sub->name);
         return err + 30;
      }
   }

   return BENCH_SUCC;
}

void benchRelease(bench_t *bench) {
   free(bench->title);
   for (size_t i = 0; i < bench->subs_count; i++) {
      free(bench->subs[i].name);
   }
   free(bench->subs);
   free(bench);
}


#define SEC 1000000000
#define MILLI_SEC 1000000
#define MICRO_SEC 1000

void displayTime(const struct timespec *start, const struct timespec *end) {
   time_t sec = end->tv_sec - start->tv_sec;
   long nano;
   if (end->tv_nsec < start->tv_nsec) {
      // If you finished "before" you're started, you finished in the next
      // second. Example: if you've started at 16:00 and finished at 15:00, you
      // didn't finish after minus one hour, you finished after 23 hours,
      // You finished in the next day.
      nano = SEC - start->tv_nsec + end->tv_nsec;
      sec--;
   } else nano = end->tv_nsec - start->tv_nsec;

   if (sec >= 60) { // Minutes:
      // We don't care about nanoseconds when talking about minutes.
      printf("%ld Minutes %ld Seconds", sec/60, sec%60);
   } else if (sec > 0) { // Seconds:
      printf("%ld.%03ld Seconds", sec, nano / MILLI_SEC);
   } else if (nano > MILLI_SEC) { // Milliseconds:
      printf("%ld.%03ld Milli", nano / MILLI_SEC, (nano % MILLI_SEC) / MICRO_SEC);
   } else if (nano > MICRO_SEC) { // Microseconds:
      printf("%ld.%03ld Micro", nano / MICRO_SEC, nano % MICRO_SEC);
   } else { // Nanoseconds:
      printf("%ld Nano", nano);
   }
}