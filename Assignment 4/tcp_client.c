/*
    Name : RIDAM HAZRA
    Roll : 19CS8040
*/

// Client sided implementation of TCP client-server model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <errno.h>

// Libraries for socket programming
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8040
#define INBUFSIZE 256
#define OUTBUFSIZE 256

int main() {
    int sockfd;
    char inBuffer[INBUFSIZE], outBuffer[OUTBUFSIZE];
    struct sockaddr_in server_addr;

    // Creating socket file descriptor using TCP Model
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Putting server information into server structure
    server_addr.sin_family = AF_INET;
    // Port must be put into network byte order
    server_addr.sin_port = htons(PORT);
    // Address for server is 127.0.0.1 so INADDR_ANY is used
    server_addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t server_len = sizeof(server_addr);

    printf("Enter the string: ");
    fgets(outBuffer, OUTBUFSIZE, stdin);
    outBuffer[strcspn(outBuffer, "\n")] = '\0';

    if (connect(sockfd, (struct sockaddr *)&server_addr, server_len) < 0)
    {
        printf("Connection Failed\n");
        return 1;
    }

    if ((send(sockfd, outBuffer, OUTBUFSIZE, 0)) < 0)
    {
        printf("Error: Sending data\n");
        return 1;
    }

    if ((recv(sockfd, inBuffer, INBUFSIZE, 0)) < 0)
    {
        printf("Error: Receiving data\n");
        return 1;
    }

    // checking the reply from server
    if (strcmp(inBuffer, "1") == 0) 
    {
        printf("The string is a palindrome\n");
        return 0;
    } 
    else
    {
        printf("The string is not a palindrome\n");
        return 0;
    }

    // Clearing the buffers
    bzero(inBuffer, INBUFSIZE);
    bzero(outBuffer, OUTBUFSIZE);
    
    // close connection
    close(sockfd);
    
    return 0;
}