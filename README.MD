# qbench
A small benchmarking "Library" for quick benchmarking in C.

## Installation
To compile as a static library: `make static` and grab the `qbench.o` and `qbench.h` files

To compile as a shared library: `make shared` and grab the `qbench.so` and `qbench.h` files

## Documentation
Full documentation is in the [wiki](https://github.com/yuriKhordal/qbench/wiki)!

## Usage
To use the benchmarking you need to include the `qbench.h` header. Creating a new benchmark is done with `createBenchmark(title)` function, each benchmark is made up of sub-benchmarks (for example testing out the same algorithm on multiple data sets, or testing multiple algorithms against one another). To register a sub within a benchmark, the `benchSub` function is used. To run a benchmark use the `benchRun(benchmark)` function.  
The output of the benchmark looks like this:
```
Benchmark Title
   Sub-Benchmark#1 name
   CPU Time: [time], Real Time: [time]
   Sub-Benchmark#2 name
   CPU Time: [time], Real Time: [time]
   Sub-Benchmark#3 name
   CPU Time: [time], Real Time: [time]
```

### Example
Benchmarking a hashtable vs a key/value tree.

```c
hashtable_init(&table);
kvtree_init(&tree);

bench_t *bench[3];
bench[0] = createBenchmark("Get value by key")
benchSub(bench[0], "Hash Table", table, initTableKeys, tableGetBench, releaseTableKeys);
benchSub(bench[0], "KV Tree", tree, initTreeKeys, treeGetBench, releaseTreeKeys);

bench[1] = createBenchmark("Edit Keys")
benchSub(bench[1], "Hash Table", table, initTableKeys, tableEditBench, releaseTableKeys);
benchSub(bench[1], "KV Tree", tree, initTreeKeys, treeEditBench, releaseTreeKeys);

bench[2] = createBenchmark("Add Keys")
benchSub(bench[2], "Hash Table", table, NULL, tableAddBench, NULL);
benchSub(bench[2], "KV Tree", tree, NULL, treeAddBench, NULL);

if (benchRun(bench[0]) != 0) {
   printf("Error: Failed to bench!");
}
if (benchRun(bench[1]) != 0) {
   printf("Error: Failed to bench!");
}
if (benchRun(bench[2]) != 0) {
   printf("Error: Failed to bench!");
}

benchRelease(bench[0]);
benchRelease(bench[1]);
benchRelease(bench[2]);

hashtable_free(&table);
kvtree_free(&tree);
```