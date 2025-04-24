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

    int choose = 0;
    int seconds;
    for (;;) {
        printf("1. Start Benchmark\n2. LAN\n3. Statistics\n>>>  ");
        scanf("%d", &choose);
        switch (choose) {
            case 1:
                printf("\nSeconds: ");
                scanf("%d", &seconds);
                benchmark(seconds);
                break;

            case 2:
                printf("Cooming soon...\n");
                break;

            case 3:
                print_avg_iterations(1);
            default:
                break;


        }
    }

    return 0;
}
