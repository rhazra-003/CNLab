// NAME: RIDAM HAZRA
// ROLL: 19CS8040

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Libraries for socket programing

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


int main()
{
    char buffer[100];          // data buffer for sending & receiving
    unsigned int port = 58040; // port client will connect to
    struct sockaddr_in server; // server address
    int sockfd;                // client socket

    // Put server information into server structure
    server.sin_family = AF_INET;

    // The port must be put into network byte order
    server.sin_port = htons(port);

    // Address for server is 127.0.0.1 so INADDR_ANY is used
    server.sin_addr.s_addr = INADDR_ANY;

    while (1)
    {
        // Enter an expression
        printf("Enter an expression / -1 to exit: ");
        fgets(buffer, 100, stdin);

        // Removes all whitespace characters in input
        buffer[strcspn(buffer, "\n")] = '\0';
        for (int i=0; i < strlen(buffer); i++)
        {
            if (buffer[i] == ' ')
            {
                for (int j = i; j < strlen(buffer); j++)
                {
                    buffer[j] = buffer[j + 1];
                }
            }
        }

        // If user enters -1, exit
        if (strcmp(buffer, "-1") == 0 || strcmp(buffer, "") == 0)
        {
            return 0;
        }

        // Get a stream socket
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
        if (sockfd < 0)
        {
            printf("Error opening socket\n");
            return 1;
        }

        // Connect to the server
        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            printf("Connection Failed\n");
            return 1;
        }

        // Send the expression to the server
        if (send(sockfd, buffer, 100, 0) == -1)
        {
            printf("Error in sending data\n");
            return 1;
        }

        // Empties buffer
        bzero(buffer, 100);

        // Receive the result from the server
        if (recv(sockfd, buffer, 100, 0) < 0)
        {
            printf("Error receiving data\n");
            return 1;
        }

        printf("The result is: %s\n", buffer);

        // Empties buffer
        bzero(buffer, 100);
        
        // Close connection with server
        close(sockfd);
    }

    return 0;
}