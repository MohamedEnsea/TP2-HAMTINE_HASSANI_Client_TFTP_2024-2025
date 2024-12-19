# TP2-HAMTINE_HASSANI_Client_TFTP_2024-2025
As part of our synthesis practical work, our aim is to create a TFTP client, leveraging RFC documentation and Wireshark captures to ensure proper implementation and functionality.

# Organisation
This project was carried out by Mohamed HAMTINE and Wilal HASSANI (2G2/TD1/TP1) and covers two 8-hour sessions. We have structured the project so that each file corresponds to a different question.

# How it works
To execute the code for each file (each question), simply write :
- gcc -o Question1 Question1.c (example for file Question1.c)
- ./Question1

# For question 1
 # Using arguments passed to the command line
    The gettftp and puttftp programs use the arguments provided on the command line to retrieve the information required for the request:
        - The mode (get or put) to specify whether to download or upload a file.
        - Server address (e.g. 127.0.0.1).
        - The name of the file to be processed (e.g. example.txt).
 # How the program works
    The program checks whether exactly three arguments are supplied (in addition to the executable name).
        - If the arguments are correct, they are displayed on the terminal for confirmation. 
                For example, if you run the following command: "./Question1 get 127.0.0.1 example.txt".
                The terminal displays :
                    Mode: get
                    Server: 127.0.0.1
                    File: example.txt
    
  - If the number of arguments is incorrect, the program displays an error message explaining the syntax to be used.
        For example, by executing an incomplete command: "./Question1 get 127.0.0.1".
        The terminal displays : "Incorrect command! Use this syntax: <get/put> <server> <file>"

# For question 2
    When we use the TFTP server provided by Moodle, it is accessible at the local IP address 127.0.0.1.
        To resolve this address, we execute the following command: "./Question2 127.0.0.1"
        The terminal displays several results, corresponding to the different possible configurations for address 127.0.0.1 :
            - IPv4 address (family = 2)
            - Socket types TCP (socktype = 1), UDP (socktype = 2) and the value zero for ai_socktype indicates that the caller will accept any socket type.
            - Protocols TCP (protocol = 6), UDP (protocol = 17), and the value zero for ai_protocol indicates that the caller will accept any protocol.

    This confirms that the TFTP server is accessible at address 127.0.0.1 using TCP and UDP protocols.

# For question 3
    In this part of the program, we resolve an IP address or domain name (such as google) and create a socket to communicate with this address. We use UDP for the socket type because TFTP (Trivial File Transfer Protocol) is a simple protocol for file transfer, and is implemented over UDP (User Datagram Protocol), as indicated in RFC 1350.

    When running the program with a domain name or IP address, for example : "./Question3 www.google.com" or even "./Question3 127.0.0.1"
        The terminal displays :
            **** Name resolution for 127.0.0.1 ****
            Socket successfully reserved.
            Found one answer: 
            **       family = 2
            **       socktype = 2
            **       protocol = 17

    The socktype = 2 and protocol = 17 indicate a UDP connection.

# For question 4
    In this part of the program, we've implemented a TFTP client capable of downloading files from a local server using the get command. The TFTP protocol, as described in RFC 1350, uses UDP (User Datagram Protocol) to transfer files in 512-byte blocks.
        1. Launching the server
        Before using the client program, you need to start a local TFTP server:
            - We've opened a terminal dedicated to the server.
            - We navigate to the server directory with : "cd serveur"
            - We start the server with the following command: "./go.sh"

        2. Client compilation and execution
            - In the main terminal, we compile the program with : "gcc -o Question4 Question4.c"
            - We test the program by downloading various files (available on the server) with the command "get".
        
        3. File transfer tests
        Here are the cases tested to validate the program's operation:
            Case 1: Text file transfer
                Command : "./Question4 get 127.0.0.1 motd.txt"
                    - The file motd.txt, available on the server, is successfully downloaded. The contents of the file are displayed in the terminal using printf("%c").
            
            Case 2: Transferring a 512-byte file
                Command : "./Question4 get 127.0.0.1 ones512"
                    - The ones512 file, available on the server, is successfully downloaded. The contents of the file are displayed in the terminal using printf("%d").
                    - The ones512 file is transferred in 1 packet. This is due to the maximum TFTP block size of 512 bytes.
            
            Case 3: Transfer of a 1024-byte file
                Command : "./Question4 get 127.0.0.1 ones1024"
                    - The ones1024 file is received in 2 packets. Each packet contains 512 bytes, in accordance with the TFTP protocol (512*2=1024).
            
            Case 4: Transferring a 2048-byte file
                Command : "./Question4 get 127.0.0.1 ones2048"
                    - The ones2048 file is received in 4 packets. Each packet contains 512 bytes, in accordance with the TFTP protocol (512*4=2048).
            
# For question 5
    In this part of the program, we've implemented a TFTP client capable of sending files to a local server using the put command.
            1. Launching the server
            Before using the client program, you need to start a local TFTP server:
                - We've opened a terminal dedicated to the server.
                - We navigate to the server directory with : "cd serveur"
                - We start the server with the following command: "./go.sh"

            2. Client compilation and execution
                - In the main terminal, we compile the program with : "gcc -o Question5 Question5.c"
                - We test the program with the command "put".
            
            3. File transfer tests
            Here are the cases tested to validate the program's operation:
                Case 1: Transfer of a text file (less than 512 bytes)
                    Command : "./Question5 put 127.0.0.1 bonjour.txt"
                        - The file bonjour.txt, available in the client directory, is successfully sent. The file "bonjour.txt" appears on the server.
                        - The file is transferred in a single data packet (512 bytes), using the TFTP protocol.
                        - Acknowledgement (ACK) from the server is correctly received, indicating that the transfer was carried out without error.
                
                Case 2: Transfer of a 1024-byte file
                    Command : "./Question5 put 127.0.0.1 alt1024"
                        - The alt1024 file, available in the client directory, is successfully sent. Indeed, the file "alt1024" appears on the server.
                        - The alt1024 file is transferred in 2 packets. This is due to the maximum size of a TFTP block, which is 512 bytes (512*2=1024).

                

                    




