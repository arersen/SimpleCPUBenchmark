#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>

#include "network/client.h"
#include "benchmark/benchmark.h"

void *client_thread(void *arg) {
    client_poll();
}
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
                char ip_address[20] = {0};
                Data client_data;

                extern unsigned long long total;
                client_data.total = &total;
                extern volatile int running;
                client_data.running = &running;

                printf("IP Address: ");
                scanf("%s", ip_address);
                connect_client_to_dedicated_server(ip_address, DEFAULT_PORT, &client_data);

                pthread_t thread;
                pthread_create(&thread, NULL, client_thread, NULL);

                benchmark(10);

                pthread_join(thread, NULL);
                break;

            case 3:
                print_avg_iterations(1);
            default:
                break;


        }
    }

    return 0;
}
