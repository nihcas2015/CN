#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    int n;

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server is listening ........\n");

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Accept an incoming connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Receive the number n from the client
    int read_size = read(new_socket, buffer, BUFFER_SIZE);
    if (read_size < 0) {
        perror("Read failed");
        close(new_socket);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Convert the received string (n) to an integer
    n = atoi(buffer);
    printf("Received number n from client: %d\n", n);

    // Seed the random number generator
    srand(time(NULL));

    // Generate n random numbers and send them to the client
    int random_numbers[n];
    for (int i = 0; i < n; i++) {
        random_numbers[i] = rand() % 100;  // Generate a random number between 0 and 99
        sprintf(buffer + strlen(buffer), "%d ", random_numbers[i]);
    }

    // Send the generated random numbers back to the client
    send(new_socket, buffer, strlen(buffer), 0);
    printf("Sent random numbers to client\n");

    // Clean up
    close(new_socket);
    close(server_fd);
    return 0;
}

