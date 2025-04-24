# Cross-Platform CPU Benchmark

A lightweight, cross-platform CPU benchmark written in C designed to test the **overall performance** of your CPU by stressing multiple aspects such as **ALU (arithmetic logic unit)**, **FPU (floating point unit)**, **memory/cache operations**, and **branch prediction**.

## Features

- **Cross-platform**: Runs on both **Linux** and **Windows**.
- **Multi-threaded**: Uses multiple CPU threads for benchmarking.
- **Realistic workload**: Simulates heavy CPU load, combining integer, floating-point, memory, and branch operations.
- **Minimal dependencies**: Written in pure C, with no external libraries required.
- **Benchmarking output**: Measures the total number of iterations across all threads within 10 seconds.

## Why Use This Benchmark?

- **General CPU performance testing**: Evaluate the raw computational power of your system.
- **Real-world stress testing**: Simulates a mixture of workloads that resemble heavy tasks like scientific computing, cryptography, and data processing.
- **Easy to run**: Just compile and run to get results quickly.

## Usage

```
[awlik@archlinux cmake-build-debug]$ ./cpu_benchmark 
CPU: 11th Gen Intel(R) Core(TM) i7-11800H @ 2.30GHz
Benchmark: running 16 threads for 15 seconds...
Total iterations in 15 seconds: 4477947
Saving results to: results/1745521041.result
[awlik@archlinux cmake-build-debug]$
```

