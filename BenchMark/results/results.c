//
// Created by awlik on 4/24/25.
//

#include "results.h"



void save_result(unsigned long long file_total, int seconds) {
    const char *result_directory_name = "results";
    struct stat st;
    if (stat(result_directory_name, &st) != 0 || !S_ISDIR(st.st_mode)) {
        if (mkdir(result_directory_name, 0755) == 0) {
            printf("Created directory results/");
        } else {
            printf("%s %d Failed to create directory.\n", __func__, __LINE__);
            exit(1);
        }
    }

    char file_name[1024];
    memset(file_name, 0, 1024);
    sprintf(file_name, "%s/%d.result", result_directory_name, time(NULL));

    FILE *file = fopen(file_name, "w"); // відкриваємо файл для запису
    if (file == NULL) {
        perror("Cant't open file for writing");
        exit(1);
    }
    fprintf(file, "%llu\n%d", file_total, seconds);
    fclose(file);
    printf("Saving results to: %s\n", file_name);
}

void print_avg_iterations(int seconds) {
    unsigned long long total_sum = 0;
    unsigned int count = 0;

    const char *result_directory_path = "./results/";
    unsigned long long result_total = 0, file_seconds = 0;
    struct dirent *entry;
    DIR *dir = opendir(result_directory_path);
    if (dir == NULL) {
        perror("opendir");
        exit(1);
    }

    char filepath[1024];
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        snprintf(filepath, sizeof(filepath), "%s/%s", result_directory_path, entry->d_name);

        FILE *file = fopen(filepath, "r");
        if (!file) {
            perror("fopen");
            continue;
        }

        if (fscanf(file, "%llu\n%llu", &result_total, &file_seconds) == 2) {
            //printf("File: %s | total = %lld, seconds = %lld\n", entry->d_name, total, file_seconds);
            count++;
            total_sum += ((double)seconds / (double)file_seconds) * (double)result_total;
        } else {
            fprintf(stderr, "Cant read data from file: %s\n", entry->d_name);
        }

        fclose(file);
    }
    closedir(dir);
    if (seconds == 1) printf("Average IPS: %llu\n", total_sum / count);
    else printf("Average IP%dS: %llu\n", seconds, total_sum / count);

}