#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address / Address not supported\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    printf("Server Connected\n");

    // Get user input string
    printf("Enter a string to send to the server: ");
    fgets(buffer, BUFFER_SIZE, stdin);  // Take user input

    // Send the string to the server
    send(sock, buffer, strlen(buffer), 0);
    printf("String sent to server: %s\n", buffer);

    // Receive the string from the server
    int read_size = read(sock, buffer, BUFFER_SIZE);
    if (read_size < 0) {
        printf("\nRead failed\n");
        return -1;
    }

    printf("Received from server: %s\n", buffer);

    // Clean up
    close(sock);
    return 0;
}

