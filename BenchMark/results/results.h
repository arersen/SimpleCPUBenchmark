//
// Created by awlik on 4/24/25.
//

#ifndef RESULTS_H
#define RESULTS_H
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
void save_result(unsigned long long total, int seconds);
void print_avg_iterations(int seconds);
#endif //RESULTS_H
