#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 1024

// Function to calculate the age based on DOB
void calculate_age(char* dob, char* result) {
    struct tm dob_tm;
    time_t now;
    struct tm* current_time;
    int years, months, days;
    
    // Parse the DOB (assuming the format is YYYY-MM-DD)
    sscanf(dob, "%d-%d-%d", &dob_tm.tm_year, &dob_tm.tm_mon, &dob_tm.tm_mday);
    dob_tm.tm_year -= 1900;  // Adjust year for tm structure
    dob_tm.tm_mon -= 1;      // Adjust month (tm struct uses 0-11 range)

    // Get the current time
    time(&now);
    current_time = localtime(&now);

    // Calculate the difference in years, months, and days
    years = current_time->tm_year - dob_tm.tm_year;
    months = current_time->tm_mon - dob_tm.tm_mon;
    days = current_time->tm_mday - dob_tm.tm_mday;

    if (days < 0) {
        months--;
        days += 30; // Approximate days in a month
    }

    if (months < 0) {
        years--;
        months += 12;
    }

    // Prepare the result string
    snprintf(result, BUFFER_SIZE, "Age: %d years, %d months, %d days", years, months, days);
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};
    char result[BUFFER_SIZE] = {0};

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
        // Receive DOB from client
        int recv_len = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addrlen);
        if (recv_len < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[recv_len] = '\0';  // Null-terminate the received string

        // Calculate the age based on the received DOB
        calculate_age(buffer, result);

        // Send the calculated age back to the client
        sendto(server_fd, result, strlen(result), 0, (struct sockaddr*)&client_addr, addrlen);
    }

    close(server_fd);
    return 0;
}

