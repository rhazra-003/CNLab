/*
    Name : RIDAM HAZRA
    Roll : 19CS8040
*/

// Server sided implementation of UDP & TCP Client-Server Model
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>

// Libraries for socket programming
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// For handling multiple socket connections
#include <sys/time.h>

// Defining max function
#define max(X, Y) (((X) > (Y)) ? (X) : (Y))

#define PORT 8040
#define INBUFSIZE 256
#define OUTBUFSIZE 256

int isPalindrome(char *s)
{
    int i, len;
    len = strlen(s);

    char *str = (char*)malloc(len * sizeof(char));

	for (i = 0; i < len; ++i) 
    {
		str[i] = tolower(s[i]);
	}

    for (i = 0; i < len / 2; i++)
    {
        if (str[i] != str[len - i - 1])
        {
            return 0;
        }
    }    
    return 1;
}

int main() 
{
    int listenfd, connectfd, udpfd;     // Socket Descriptors
    int nReady, maxfdp1;
    char inBuffer[INBUFSIZE], outBuffer[OUTBUFSIZE];
    struct sockaddr_in client_addr, server_addr;
    
    pid_t childPid;
    fd_set rset;
    int n;
    socklen_t client_len;
    void sig_chld(int);

    // Creating and listening TCP socket
    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
        printf("TCP socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    bzero(&server_addr, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET;       // IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    // Binding the server address structure to TCP listenfd 
    if (bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("ERROR: Binding TCP socket\n");
        exit(EXIT_FAILURE);
    }

    listen(listenfd, 10);

    // Creating UDP socket
    udpfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (udpfd < 0)
    {
        printf("ERROR: Creating UDP socket\n");
        exit(EXIT_FAILURE);
    }

    // Binding the server address structure to UDP sockfd
    if (bind(udpfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("ERROR: Binding UDP socket");
        exit(EXIT_FAILURE);
    }

    // Clears the descriptor set
    FD_ZERO(&rset);

    // Get maxfd
    maxfdp1 = max(listenfd, udpfd) + 1;

    while(1)
    {

        // Set listenfd and udpfd to readset
        FD_SET(listenfd, &rset);
        FD_SET(udpfd, &rset);

        // Select the ready descriptor
        nReady = select(maxfdp1, &rset, NULL, NULL, NULL);

        /* If TCP socket is readable then handle it by accepting the connection */
        if (FD_ISSET(listenfd, &rset))
        {
            client_len = sizeof(client_addr);
            connectfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_len);
    
            if(connectfd < 0)
            {
                printf("ERROR: Accept error\n");
                exit(EXIT_FAILURE);
            }

            bzero(inBuffer, INBUFSIZE);
            
            if (recv(connectfd, inBuffer, INBUFSIZE, 0) < 0)
            {
                printf("ERROR: receiving data\n");
                exit(EXIT_FAILURE);
            }

            printf("\nBuffer string (TCP): %s\n", inBuffer);

            // Check for palindrome
            bzero(outBuffer, OUTBUFSIZE);
            
            if(isPalindrome(inBuffer))
            {
                strcpy(outBuffer, "1");
            }
            else
            {
                strcpy(outBuffer, "0");
            }
            
            if(send(connectfd, outBuffer, OUTBUFSIZE, 0) < 0)
            {
                printf("ERROR: Sending Data\n");
                exit(EXIT_FAILURE);
            }

            // Clearing the buffers
            bzero(inBuffer, INBUFSIZE);
            bzero(outBuffer, OUTBUFSIZE);
            close(connectfd);
        }

        // If UDP socket is readable receive the message
        if (FD_ISSET(udpfd, &rset))
        {
            client_len = sizeof(client_addr);
            bzero(inBuffer, INBUFSIZE);
            
            if ((n = recvfrom(udpfd, inBuffer, INBUFSIZE, 0, (struct sockaddr *)&client_addr, &client_len)) < 0)
            {
                printf("ERROR: Receiving data\n");
                exit(EXIT_FAILURE);
            }
            
            printf("\nBuffer string (UDP): %s", inBuffer);

            // Check for palindrome
            bzero(outBuffer, OUTBUFSIZE);
            if(isPalindrome(inBuffer))
            {
                strcpy(outBuffer, "1");
            }
            else
            {
                strcpy(outBuffer, "0");
            }
            
            if (sendto(udpfd, outBuffer, OUTBUFSIZE, 0, (struct sockaddr *)&client_addr, client_len) < 0)
            {
                printf("ERROR: sending data\n");
                exit(EXIT_FAILURE);
            }

            // Clearing the buffers
            bzero(inBuffer, INBUFSIZE);
            bzero(outBuffer, OUTBUFSIZE);
        }
    }

    return 0;
}