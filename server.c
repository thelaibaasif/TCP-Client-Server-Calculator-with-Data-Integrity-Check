#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

float calculate(float num1, float num2, char* operation) {
    if (strcmp(operation, "add") == 0)
        return num1 + num2;
    else if (strcmp(operation, "subtract") == 0)
        return num1 - num2;
    else if (strcmp(operation, "multiply") == 0)
        return num1 * num2;
    else if (strcmp(operation, "divide") == 0) {
        if (num2 == 0)
            return -1; // Cannot divide by zero
        return num1 / num2;
    }
    else if (strcmp(operation, "modulus") == 0)
        return (int)num1 % (int)num2;
    else
        return -2; // Invalid operation
}

unsigned char calculate_checksum(char *data, int len) {
    unsigned char checksum = 0;
    for (int i = 0; i < len; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    float num1, num2, result;
    char operation[20];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 12345
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 12345
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        while (1) {
            valread = read(new_socket, buffer, BUFFER_SIZE);
            if (valread == 0)
                break;

            sscanf(buffer, "%f,%f,%s", &num1, &num2, operation);

            // Calculate checksum of received data
            unsigned char received_checksum = buffer[valread];
            unsigned char calculated_checksum = calculate_checksum(buffer, valread);

            if (received_checksum != calculated_checksum) {
                printf("Data integrity check failed. Ignoring request.\n");
                break;
            }

            result = calculate(num1, num2, operation);
            sprintf(buffer, "%.2f", result);

            // Calculate checksum for response
            unsigned char response_checksum = calculate_checksum(buffer, strlen(buffer));
            buffer[strlen(buffer)] = response_checksum;

            send(new_socket, buffer, strlen(buffer) + 1, 0);
            memset(buffer, 0, sizeof(buffer));
        }
        close(new_socket);
    }
    return 0;
}
