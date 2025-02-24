#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to encode the message using Caesar cipher
void encode_message(char* message, char* encoded_message, int shift) {
    for (int i = 0; message[i] != '\0'; i++) {
        // Encoding uppercase letters
        if (message[i] >= 'A' && message[i] <= 'Z') {
            encoded_message[i] = ((message[i] - 'A' + shift) % 26) + 'A';
        }
        // Encoding lowercase letters
        else if (message[i] >= 'a' && message[i] <= 'z') {
            encoded_message[i] = ((message[i] - 'a' + shift) % 26) + 'a';
        }
        // Non-alphabetical characters remain unchanged
        else {
            encoded_message[i] = message[i];
        }
    }
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char encoded_message[BUFFER_SIZE] = {0};
    int shift = 3;  // Caesar cipher shift

    // Create UDP socket
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // Infinite loop to handle multiple clients
    while (1) {
        // Receive message from client
        int recv_len = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addrlen);
        if (recv_len < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[recv_len] = '\0';  // Null-terminate the received string

        // Encode the message
        encode_message(buffer, encoded_message, shift);

        // Print the encoded message
        printf("Encoded Message: %s\n", encoded_message);

        // Send the encoded message back to the client
        sendto(server_fd, encoded_message, strlen(encoded_message), 0, (struct sockaddr*)&client_addr, addrlen);
    }

    close(server_fd);
    return 0;
}


