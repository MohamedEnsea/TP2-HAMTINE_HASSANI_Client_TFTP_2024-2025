#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

// The main function starts here, which takes command line arguments
int main(int argc, char ** argv) {
    printf("**** Name resolution for %s ****\n", argv[1]); // We print a message showing the hostname we're resolving

    struct addrinfo hints; // We create a variable 'hints' to define the criteria for the address lookup
    memset(&hints,0,sizeof(struct addrinfo)); // Initialize the 'hints' structure to zero

    struct addrinfo * res; // Pointer to store the results from getaddrinfo
    getaddrinfo(argv[1],NULL,&hints,&res); // We call getaddrinfo to resolve the given hostname (argv[1]) and store the result in 'res'

    struct addrinfo * current = res; // We create a pointer 'current' to loop through the results

    // We loop through all the results and print information about each one
    while(current != NULL){
        printf("Found one answer : \n");
        printf("**\t family = %d\n",current->ai_family); // Print the address family (IPv4, IPV6)
        printf("**\t socktype = %d\n", current->ai_socktype); // Print the socket type
        printf("**\t protocol = %d\n", current->ai_protocol); // Print the protocol (17 for UDP, 6 for TCP, 0 to use any protocol)

        current=current->ai_next; // We move to the next address in the list, if available
    }
    return 0;
}