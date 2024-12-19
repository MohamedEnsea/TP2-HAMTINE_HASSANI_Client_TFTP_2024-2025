#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>  

int main(int argc, char **argv) {
    printf("**** Name resolution for %s ****\n", argv[1]);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));

    // We specify the resolution criteria
    hints.ai_family = AF_INET; // We use IPv4
    hints.ai_socktype = SOCK_DGRAM; // We set the socket to use UDP, which is the protocol used by TFTP

    struct addrinfo *res;
    getaddrinfo(argv[1], NULL, &hints, &res);

    struct addrinfo *current = res;

    int sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol);

    printf("Socket successfully reserved.\n"); // We print a message confirming that the socket has been successfully reserved

    while (current != NULL) {
        printf("Found one answer: \n");
        printf("**\t family = %d\n", current->ai_family);
        printf("**\t socktype = %d\n", current->ai_socktype);
        printf("**\t protocol = %d\n", current->ai_protocol);

        current = current->ai_next;
    }

    close(sockfd);

    return 0;
}
