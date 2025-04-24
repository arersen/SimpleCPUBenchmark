//
// Created by awlik on 4/24/25.
//
#include "cpu_info.h"

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