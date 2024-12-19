#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define TFTP_SIZE 516 // Maximum size for TFTP packets (4 bytes header + 512 bytes of data)
#define SERVER_PORT "1069" // Standard TFTP port
#define OCTET_MODE "octet"

#define OPCODE_LENGTH 2

#define ACK_OPCODE 4

#define ACK_LENGTH 4

int main(int argc, char *argv[]) {

    // We check if the user gave exactly 3 arguments
    if (argc != 4) {
        fprintf(stderr, "Incorrect command ! Use this syntax: <get> <server> <file>\n");
        return 1;
    }

    // We get the mode (get), the server address, and the file name  
    char *mode = argv[1];  // Mode: "get"
    char *server = argv[2];  // Server address
    char *file = argv[3];  // File name to send or receive

    // We print the arguments to confirm them
    printf("Mode: %s\n", mode);
    printf("Server: %s\n", server);
    printf("File: %s\n", file);

    // We initialize the structures to resolve the server address
    struct addrinfo hints;
    struct addrinfo *current;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;  // We use IPv4
    hints.ai_socktype = SOCK_DGRAM;  // TFTP uses UDP
    hints.ai_protocol = IPPROTO_UDP;

    // We resolve the server address
    getaddrinfo(server, SERVER_PORT, &hints, &current);

    // We create the socket
    int sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);

    // We build the RRQ (Read Request) packet
    char request_buffer[TFTP_SIZE];
    memset(request_buffer, 0, TFTP_SIZE);  // Clear the buffer
    request_buffer[0] = 0;  // Opcode for RRQ
    request_buffer[1] = 1;  // OpCode for RRQ 

    strcpy(request_buffer + 2, file);  // We add the filename
    size_t filename_length = strlen(request_buffer + 2);

    strcpy(request_buffer + 2 + filename_length + 1, OCTET_MODE); // We add the "octet" mode
    size_t mode_length = strlen(request_buffer + 3 + filename_length);

    // We send the RRQ
    if (sendto(sockfd, request_buffer, filename_length + mode_length + OPCODE_LENGTH + 2, 0, current->ai_addr, current->ai_addrlen) == -1) {
        perror("Error: Could not send the RRQ");
        exit(EXIT_FAILURE);
    }
    printf("Read request sent successfully.\n");

    // Start receiving the file in data packets
    while (1) {
        // We receive the data packet
        int received_bytes = recvfrom(sockfd, request_buffer, TFTP_SIZE, 0, current->ai_addr, &current->ai_addrlen);
        
        // We print the file data
        printf("Data received from file:\n"); 
        for (int i = 4; i < received_bytes; i++) 
        {
            printf("%c", request_buffer[i]); // We use %c to print characters as letters, and %d to print numbers.
        }
        printf("\n");

        // We send the ACK (Acknowledgment)
        request_buffer[1] = ACK_OPCODE;  // We set the opcode to ACK
        if (sendto(sockfd, request_buffer, ACK_LENGTH, 0, current->ai_addr, current->ai_addrlen) == -1) {
            perror("Error: Could not send the ACK");
            exit(EXIT_FAILURE);
        }
        printf("Acknowledgment sent successfully.\n");

        // If the data packet is smaller than 512 bytes, it is the last packet
        if (received_bytes < TFTP_SIZE) {
            printf("End of transmission: all packets received.\n");
            break;
        }
    }

    // We close the socket
    close(sockfd);

    printf("Connection closed.\n");
    exit(EXIT_SUCCESS);
}
