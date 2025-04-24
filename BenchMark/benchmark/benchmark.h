//
// Created by awlik on 4/24/25.
//
#pragma once
#ifndef BENCHMARK_H
#define BENCHMARK_H

typedef struct {
    unsigned long long counter;
} ThreadData;

#include <stdint.h>
#include <math.h>


#ifdef _WIN32
#include <windows.h>
DWORD WINAPI worker(LPVOID arg);
#else
#include <pthread.h>
void* worker(void* arg);
#endif

void benchmark();
#include <malloc.h>
#include "../cpu_info/cpu_info.h"
#endif //BENCHMARK_H
