//
// Created by awlik on 4/25/25.
//
#include "server.h"


void start_dedicated_server(int port) {
    int server_fd, client1, client2;
    struct sockaddr_in address;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("server: socket failed\n");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
    listen(server_fd, 2);

    printf("server: waiting for connections...\n");

    socklen_t len = sizeof(address);

    printf("Waiting for client1...\n");
    client1 = accept(server_fd, (struct sockaddr*)&address, &len);

    printf("Waiting for client2...\n");
    client2 = accept(server_fd, (struct sockaddr*)&address, &len);

    int ready = 0;
    while (!ready) {
        printf("Waiting for a ready client1...\n");
        read(client1, &ready, sizeof(ready));
    }

    ready = 0;
    while (!ready) {
        printf("Waiting for a ready client2...\n");
        read(client2, &ready, sizeof(ready));
    }

    Data client1_data;
    Data client2_data;

    while (1) {
        read(client1, &client1_data, sizeof(client1_data));
        send(client2, &client1_data, sizeof(client1_data), 0);
        if (!client1_data.running) break;

        read(client2, &client2_data, sizeof(client2_data));
        send(client1, &client2_data, sizeof(client2_data), 0);
        if (!client2_data.running) break;
    }
    printf("Benchmark end:\n\tclient1_total: %llu\n\tclient2_total: %llu\n", client1_data.total, client2_data.total);
}
