//
// Created by awlik on 4/26/25.
//

#include "network/client.h"

int main() {
    char ip_address[20] = {0};
    Data client_data;
    printf("IP Address: ");
    scanf("%s", ip_address);
    connect_client_to_dedicated_server(ip_address, DEFAULT_PORT, &client_data);
}
