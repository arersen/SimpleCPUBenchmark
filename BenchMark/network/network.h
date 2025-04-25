//
// Created by awlik on 4/25/25.
//
#pragma once
#ifndef NETWORK_H
#define NETWORK_H

#define DEFAULT_PORT 55555
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
typedef struct {
    unsigned long long *total;
    volatile int *running;
} Data;

#include "client.h"
#include "server.h"
extern void connect_client_to_dedicated_server(const char* ip_address, int port, Data* client_data_ptr);
extern void start_dedicated_server(int port);

#endif //NETWORK_H
