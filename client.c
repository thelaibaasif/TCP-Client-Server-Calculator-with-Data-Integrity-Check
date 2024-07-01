#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define BUFFER_SIZE 1024

unsigned char calculate_checksum(char *data, int len) {
    unsigned char checksum = 0;
    for (int i = 0; i < len; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};
    float num1, num2;
    char operation[20];

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while (1) {
        printf("Enter two numbers (comma separated): ");
        scanf("%f,%f", &num1, &num2);
        printf("Enter operation (add/subtract/multiply/divide/modulus): ");
        scanf("%s", operation);

        // Create a formatted string containing the numbers and operation
        sprintf(buffer, "%.2f,%.2f,%s", num1, num2, operation);

        // Calculate checksum for the data
        unsigned char checksum = calculate_checksum(buffer, strlen(buffer));

        // Append the checksum to the end of the buffer
        sprintf(buffer + strlen(buffer), "%c", checksum);

        // Send the data with checksum to the server
        send(sock, buffer, strlen(buffer) + 1, 0);

        // Receive the response from the server
        valread = read(sock, buffer, BUFFER_SIZE);

        // Check if the response is empty
        if (valread == 0) {
            printf("Server disconnected.\n");
            break;
        }

        // Print the output received from the server
        printf("Output: %s\n", buffer);

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
    }

    // Close the socket
    close(sock);
    return 0;
}
