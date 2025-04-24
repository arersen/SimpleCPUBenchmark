#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>


#include "benchmark/benchmark.h"


int main() {
    benchmark(15);
    return 0;
}
