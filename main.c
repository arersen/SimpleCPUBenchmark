#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
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
#include <windows.h>
#include <intrin.h>
void print_cpu_info() {
    int cpu_info[4] = { 0 };
    char cpu_brand[0x40] = { 0 };

    __cpuid(cpu_info, 0x80000000);
    unsigned int nExIds = cpu_info[0];

    if (nExIds >= 0x80000004) {
        __cpuid((int*)(cpu_info), 0x80000002);
        memcpy(cpu_brand, cpu_info, sizeof(cpu_info));
        __cpuid((int*)(cpu_info), 0x80000003);
        memcpy(cpu_brand + 16, cpu_info, sizeof(cpu_info));
        __cpuid((int*)(cpu_info), 0x80000004);
        memcpy(cpu_brand + 32, cpu_info, sizeof(cpu_info));
    }

    printf("CPU: %s\n", cpu_brand);
}
#else
#include <string.h>
void print_cpu_info() {
    FILE* fp = fopen("/proc/cpuinfo", "r");
    if (!fp) {
        perror("fopen");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "model name")) {
            char* name = strchr(line, ':');
            if (name) {
                printf("CPU:%s", name + 1);
                break;
            }
        }
    }

    fclose(fp);
}
#endif
#ifdef _WIN32
#include <windows.h>
DWORD WINAPI worker(LPVOID arg);
#else
#include <pthread.h>
void* worker(void* arg);
#endif

volatile int running = 1;

typedef struct {
    unsigned long long counter;
} ThreadData;

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

#ifdef _WIN32
int get_cpu_threads() {
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    return sysinfo.dwNumberOfProcessors;
}
#else
#include <unistd.h>
int get_cpu_threads() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}
#endif

void sleep_exact_seconds(int sec) {
#ifdef _WIN32
    Sleep(sec * 1000);
#else
    struct timespec ts = { sec, 0 };
    nanosleep(&ts, NULL);
#endif
}

int main() {
    int num_threads = get_cpu_threads();
    print_cpu_info();
    printf("Benchmark: running %d threads for 10 seconds...\n", num_threads);

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

    sleep_exact_seconds(10);
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

    printf("Total iterations in 10 seconds: %llu\n", total);

    free(data);
    free(threads);
    return 0;
}

