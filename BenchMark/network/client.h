//
// Created by awlik on 4/26/25.
//

#ifndef CLIENT_H
#define CLIENT_H
#include "network.h"
void connect_client_to_dedicated_server(const char* ip_address, int port, Data* client_data_ptr);
void client_poll();
#include <time.h>
#define SAMPLING_PERIOD 1000
#endif //CLIENT_H
