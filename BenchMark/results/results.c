//
// Created by awlik on 4/24/25.
//

#include "results.h"



void save_result(unsigned long long total, int seconds) {
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
    fprintf(file, "%llu\n%d", total, seconds);
    fclose(file);
    printf("Saving results to: %s\n", file_name);
}
