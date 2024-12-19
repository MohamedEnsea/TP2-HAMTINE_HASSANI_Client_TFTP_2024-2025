#include <stdio.h>
#include <stdlib.h>

// We start the main function with arguments from the command line
int main(int argc, char *argv[]) {
    
    // We check if the user gave exactly 3 arguments
    if (argc != 4) {
        fprintf(stderr, "Incorrect command ! Use this syntax: <get/put> <server> <file>\n"); // We show an error if the command is not correct
        return 1; // We stop the program with an error code
    }

// We get the mode (get or put), the server address, and the file name
    char *mode = argv[1];
    char *server = argv[2];
    char *file = argv[3];

// We print the arguments to confirm them
    printf("Mode: %s\n", mode); // Example: "Mode: get" or "Mode: put"
    printf("Server: %s\n", server); // Example: "Server: 127.0.0.1"
    printf("File: %s\n", file); // Example: "File: example.txt"

    return 0;
}