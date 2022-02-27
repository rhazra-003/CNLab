/*
    Name : RIDAM HAZRA
    Roll : 19CS8040
*/

// Client sided implementation of UDP client-server model
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

int main()
{
    int sockfd;
    char inBuffer[INBUFSIZE], outBuffer[OUTBUFSIZE];
    struct sockaddr_in server_addr;

    // Creating socket file descriptor using UDP Model
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("ERROR: creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));

    // Filling server information
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int n;
    socklen_t server_len = sizeof(server_addr);

    printf("Enter the string: ");
    fgets(outBuffer, OUTBUFSIZE, stdin);
    outBuffer[strcspn(outBuffer, "\n")] = '\0';

    if (strcmp(outBuffer, "-1") == 0)
    {
        return 0;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, server_len) < 0)
    {
        printf("ERROR: Connection failed\n");
        return 1;
    }

    if (sendto(sockfd, outBuffer, OUTBUFSIZE, 0, (const struct sockaddr *)&server_addr, server_len) < 0)
    {
        printf("ERROR: Sending data\n");
        return 1;
    }

    if ((n = recvfrom(sockfd, inBuffer, INBUFSIZE, 0, (struct sockaddr *)&server_addr, &server_len)) < 0)
    {
        printf("ERROR: Receiving data\n");
        return 1;
    }


    // Checking the reply from server
    if (strcmp(inBuffer, "1") == 0)
    {
        printf("The provided string '%s' is a palindrome\n", outBuffer);
        return 0;
    }
    else
    {
        printf("The provided string '%s' is not a palindrome\n", outBuffer);
        return 0;
    }

    // Clearing the buffers
    bzero(inBuffer, INBUFSIZE);
    bzero(outBuffer, OUTBUFSIZE);
    close(sockfd);

    return 0;
}