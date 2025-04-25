//
// Created by awlik on 4/26/25.
//

#include "client.h"

#include "server.h"

Data client2_data;

int server;
Data* extern_client_data_ptr;

void connect_client_to_dedicated_server(const char* ip_address, int port, Data* client_data_ptr) {
    struct sockaddr_in serv_addr;
    int ready = 0;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    inet_pton(AF_INET, ip_address, &serv_addr.sin_addr);

    connect(server, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    printf("Type 1 for ready: ");
    while (!ready) {
        scanf("%d", &ready);
        send(server, &ready, sizeof(ready), 0);
    }
    extern_client_data_ptr = client_data_ptr;




}

void client_poll() {
    clock_t counter = clock();
    while (1) {
        if (clock() > counter + SAMPLING_PERIOD) {
            send(server, extern_client_data_ptr, sizeof(Data), 0);
            read(server, &client2_data, sizeof(Data));
            if (!extern_client_data_ptr->running) break;
            counter = clock();
        }
    }
}