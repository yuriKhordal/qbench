#ifndef __QBENCH_H__
#define __QBENCH_H__

#include<stddef.h>

/**
 * @brief A pointer to one of the sub-benchmarking functions.
 * @param data Additional data for the function to use while being executed.
 * @return Returns 0 on success and a negative integer on failure.
 */
typedef int (*bench_func_t)(void *data);

/**
 * Represents a single sub-benchmark of a benchmark and holds an initialization
 * function, a benchmarking function, and a release function.
 */
struct bench_sub_t {
   char *name;
   void *data;
   bench_func_t init;
   bench_func_t bench;
   bench_func_t release;
};

/**
 * A benchmarking object. Represents the benchmark and holds data about it's
 * sub-benchmarks and their methods. Used in all the benchmarking functions.
 */
typedef struct {
   char *title;
   struct bench_sub_t *subs;
   size_t subs_size;
   size_t subs_count;
} bench_t;

/**
 * @brief Create a new benchmark object.
 * @param title The title of the benchmark to display when benchmarking.
 * @return A benchmark object that is needed for the rest of the
 * benchmark functions.
 */
bench_t * createBenchmark(const char *title);

/**
 * @brief Register a new sub-benchmark with a specified benchmark.
 * @param bench The benchmark to register the sub into.
 * @param name The name of the sub-benchmark to display after benchmarking.
 * @param data Additional data to pass around when initializing,
 * benchmarking, and releasing. If none needed, can be NULL.
 * @param init An initialization function to call on the sub-benchmark's data
 * BEFORE benchmarking. If no initialization is need, can be NULL.
 * @param benchmark The benchmark function to measure and display the time of.
 * @param release A function to realease the sub-benchmark's data's memory if
 * any was allocated. If no releasing is need, can be NULL.
 * @return Returns 0 on success or a negative number in case of a memory error.
 */
int benchSub(bench_t *bench, const char *name, void *data,
bench_func_t init, bench_func_t benchmark, bench_func_t release);

/**
 * @brief Run and measure a benchmark and display results of all the
 * sub-benchmarks to stdout.
 * @param bench The benchmark object to run.
 * @return Returns 0 on success, or the return value of the first function
 * that fails.
 * In case of failure no further functions in the benchmark are run.
 */
int benchRun(const bench_t *bench);

/**
 * @brief Releases all the memory of the benchmark.
 * @param bench The benchmark to release the memory of.
 * @note The data of the sub-benchmarks is not touched. Releasing it is the
 * user's responsibility.
 */
void benchRelease(bench_t *bench);

#endif