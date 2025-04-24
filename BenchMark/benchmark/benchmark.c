//
// Created by awlik on 4/24/25.
//

#include "benchmark.h"

volatile int running = 1;

static void sleep_exact_seconds(int sec) {
#ifdef _WIN32
    Sleep(sec * 1000);
#else
    struct timespec ts = { sec, 0 };
    nanosleep(&ts, NULL);
#endif
}

void benchmark_iteration_complex() {
    // Integer math
    int a = 1, b = 1234567, c = 89;
    for (int i = 0; i < 100; ++i) {
        a = (a ^ b) + c;
        b = (b << 1) | (b >> 31);
        c = (c * 3 + a) % (i + 1 + 1);
    }

    // Floating point math
    double x = 0.0001;
    for (int i = 0; i < 100; ++i) {
        x = sin(x) + sqrt(x * x + 1.0);
        x = log(x + 1.00001);
    }

    // Memory access — simulate cache use
    static uint8_t buffer[4096];
    for (int i = 0; i < sizeof(buffer); ++i) {
        buffer[i] = (buffer[i] + i) ^ 0xAA;
    }

    // Branching
    int res = 0;
    for (int i = 0; i < 100; ++i) {
        if ((i % 7) == 0)
            res += i;
        else if ((i % 5) == 0)
            res -= i;
        else
            res ^= i;
    }

    // Prevent compiler from optimizing it all out
    volatile int dummy = a + b + c + (int)x + res + buffer[123];
    (void)dummy;
}

#ifdef _WIN32
DWORD WINAPI worker(LPVOID arg) {
    ThreadData* data = (ThreadData*)arg;
    data->counter = 0;
    while (running) {
        benchmark_iteration_complex();
        data->counter++;
    }
    return 0;
}
#else
void* worker(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    data->counter = 0;
    while (running) {
        benchmark_iteration_complex();
        data->counter++;
    }
    return NULL;
}
#endif


void benchmark(int seconds) {
    running = 1;
    int num_threads = get_cpu_threads();
    print_cpu_info();
    printf("Benchmark: running %d threads for %d seconds...\n", num_threads, seconds);

    ThreadData* data = (ThreadData*)malloc(sizeof(ThreadData) * num_threads);

#ifdef _WIN32
    HANDLE* threads = (HANDLE * )malloc(sizeof(HANDLE) * num_threads);
#else
    pthread_t* threads = malloc(sizeof(pthread_t) * num_threads);
#endif

    for (int i = 0; i < num_threads; ++i) {
#ifdef _WIN32
        threads[i] = CreateThread(NULL, 0, worker, &data[i], 0, NULL);
#else
        pthread_create(&threads[i], NULL, worker, &data[i]);
#endif
    }

    sleep_exact_seconds(seconds);
    running = 0;

    for (int i = 0; i < num_threads; ++i) {
#ifdef _WIN32
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
#else
        pthread_join(threads[i], NULL);
#endif
    }

    unsigned long long total = 0;
    for (int i = 0; i < num_threads; ++i) {
        total += data[i].counter;
    }
    const char *results_directory_name = "results";

    printf("Total iterations in %d seconds: %llu\n", seconds, total);
    save_result(total, seconds);
    free(data);
    free(threads);
}