#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to decode the message using Caesar cipher
void decode_message(char* encoded_message, char* decoded_message, int shift) {
    for (int i = 0; encoded_message[i] != '\0'; i++) {
        // Decoding uppercase letters
        if (encoded_message[i] >= 'A' && encoded_message[i] <= 'Z') {
            decoded_message[i] = ((encoded_message[i] - 'A' - shift + 26) % 26) + 'A';
        }
        // Decoding lowercase letters
        else if (encoded_message[i] >= 'a' && encoded_message[i] <= 'z') {
            decoded_message[i] = ((encoded_message[i] - 'a' - shift + 26) % 26) + 'a';
        }
        // Non-alphabetical characters remain unchanged
        else {
            decoded_message[i] = encoded_message[i];
        }
    }
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE] = {0};
    char decoded_message[BUFFER_SIZE] = {0};
    int shift = 3;  // Caesar cipher shift

    // Create UDP socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    // Convert address from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        printf("\nInvalid address / Address not supported\n");
        return -1;
    }

    // Ask the user for a message and send it to the server
    printf("Enter your message: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // Remove newline character from input

    // Send message to the server
    sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Receive the encoded message from the server
    int len = sizeof(server_addr);
    int n = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &len);
    buffer[n] = '\0';  // Null-terminate the received string

    // Decode the message
    decode_message(buffer, decoded_message, shift);

    // Print the decoded message
    printf("Server Response (Decoded): %s\n", decoded_message);

    // Clean up
    close(sock);
    return 0;
}

