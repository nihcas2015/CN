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
    int n, sum = 0;

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

    // Get the number n from the user
    printf("Enter a number n: ");
    scanf("%d", &n);

    // Send the number n to the server
    sprintf(buffer, "%d", n);
    send(sock, buffer, strlen(buffer), 0);
    printf("Sent number n to server: %d\n", n);

    // Receive random numbers from the server
    int read_size = read(sock, buffer, BUFFER_SIZE);
    if (read_size < 0) {
        printf("\nRead failed\n");
        return -1;
    }

    // Print received random numbers
    printf("Received random numbers: %s\n", buffer);

    // Parse and calculate the sum of the received random numbers
    char *token = strtok(buffer, " ");
    while (token != NULL) {
        sum += atoi(token);
        token = strtok(NULL, " ");
    }

    // Print the sum of the random numbers
    printf("Sum of random numbers: %d\n", sum);

    // Clean up
    close(sock);
    return 0;
}

