#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define TFTP_SIZE 516 // Maximum size for TFTP packets (4 bytes header + 512 bytes of data)
#define SERVER_PORT "1069" // Standard TFTP port
#define OCTET_MODE "octet"

#define OPCODE_LENGTH 2

#define WRQ_OPCODE 2
#define DAT_OPCODE 3
#define ACK_OPCODE 4

#define ACK_LENGTH 4
#define DAT_LENGTH 512

int main(int argc, char *argv[]) {

    // We check if the user gave exactly 3 arguments
    if (argc != 4) {
        fprintf(stderr, "Incorrect command! Use this syntax: <put> <server> <file>\n");
        return 1;
    }

    // We get the mode (put), the server address, and the file name  
    char *mode = argv[1];  // Mode: "put"
    char *server = argv[2];  // Server address
    char *file = argv[3];  // File name to send

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

    // We open the file to send
    int filefd = open(file, O_RDONLY);
    if (filefd == -1) {
        perror("Error: Could not open file");
        exit(EXIT_FAILURE);
    }

    // We build the WRQ (Write Request) packet
    char request_buffer[TFTP_SIZE];
    memset(request_buffer, 0, TFTP_SIZE);  // Clear the buffer
    request_buffer[0] = 0;  // Opcode for WRQ
    request_buffer[1] = WRQ_OPCODE;  // OpCode for WRQ 

    strcpy(request_buffer + 2, file);  // We add the filename
    size_t filename_length = strlen(request_buffer + 2);

    strcpy(request_buffer + 2 + filename_length + 1, OCTET_MODE); // We add the "octet" mode
    size_t mode_length = strlen(request_buffer + 3 + filename_length);

    // We send the WRQ
    if (sendto(sockfd, request_buffer, filename_length + mode_length + OPCODE_LENGTH + 2, 0, current->ai_addr, current->ai_addrlen) == -1) {
        perror("Error: Could not send the WRQ");
        exit(EXIT_FAILURE);
    }
    printf("Write request sent successfully.\n");

    // Receive the acknowledgment (ACK) for the WRQ
    if (recvfrom(sockfd, request_buffer, ACK_LENGTH, 0, current->ai_addr, &current->ai_addrlen) == -1) {
        perror("Error: Could not receive ACK for WRQ");
        exit(EXIT_FAILURE);
    }
    printf("Received ACK for WRQ.\n");

    // Start sending data packets
    int block_num = 1;
    while (1) {
        // We read a block of data from the file
        ssize_t bytes_read = read(filefd, request_buffer + 4, DAT_LENGTH);
        if (bytes_read == -1) {
            perror("Error: Could not read from file");
            exit(EXIT_FAILURE);
        }

        // If no more data to send, we are done
        if (bytes_read == 0) {
            printf("End of file: no more data to send.\n");
            break;
        }

        // We set the opcode for DAT (Data packet)
        request_buffer[0] = 0;
        request_buffer[1] = DAT_OPCODE;

        // We set the block number (2 bytes)
        request_buffer[2] = (block_num >> 8) & 0xFF; // High byte
        request_buffer[3] = block_num & 0xFF; // Low byte

        // We send the data packet
        if (sendto(sockfd, request_buffer, bytes_read + 4, 0, current->ai_addr, current->ai_addrlen) == -1) {
            perror("Error: Could not send data packet");
            exit(EXIT_FAILURE);
        }
        printf("Sent data packet %d.\n", block_num);

        // We wait for the ACK for the current data packet
        int ack_received = 0;
        while (!ack_received) {
            ssize_t ack_size = recvfrom(sockfd, request_buffer, ACK_LENGTH, 0, current->ai_addr, &current->ai_addrlen);
            if (ack_size == -1) {
                perror("Error: Could not receive ACK for data packet");
                exit(EXIT_FAILURE);
            }

            // We check if the ACK corresponds to the current block number
            if (request_buffer[2] == (block_num >> 8) && request_buffer[3] == (block_num & 0xFF)) {
                printf("Received ACK for data packet %d.\n", block_num);
                ack_received = 1;
            }
        }

        // We increment the block number for the next packet
        block_num++;
    }

    // We close the file and the socket
    close(filefd);
    close(sockfd);

    printf("File transfer complete. Connection closed.\n");
    exit(EXIT_SUCCESS);
}
