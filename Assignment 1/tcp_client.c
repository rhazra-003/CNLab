/*
    RIDAM HAZRA
    ROLL NO. : 19CS8040
*/

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
    char buff[100];            
    unsigned int port = 58040; 
    struct sockaddr_in server; 
    int sockfd;                

    // Put server information into server structure
    server.sin_family = AF_INET;
    // The port must be put into network byte order
    server.sin_port = htons(port);
    // Address for server is 127.0.0.1 so INADDR_ANY is used
    server.sin_addr.s_addr = INADDR_ANY;

    while (1)
    {
        
        printf("Enter an expression / -1 to exit: ");
        fgets(buff, 100, stdin);

        
        buff[strcspn(buff, "\n")] = '\0';
        for (int i = 0; i < strlen(buff); i++)
            if (buff[i] == ' ')
                for (int j = i; j < strlen(buff); j++)
                    buff[j] = buff[j + 1];

        
        if (strcmp(buff, "-1") == 0 || strcmp(buff, "") == 0)
            return 0;

        
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
        {
            printf("Error opening socket\n");
            return 1;
        }
        
        if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
        {
            printf("Connection Failed\n");
            return 1;
        }

        
        if (send(sockfd, buff, 100, 0) == -1)
        {
            printf("Error in sending data\n");
            return 1;
        }
        
        bzero(buff, 100);

        
        if (recv(sockfd, buff, 100, 0) < 0)
        {
            printf("Error receiving data\n");
            return 1;
        }
        printf("The result is: %s\n", buff);
        
        bzero(buff, 100);
        // Close connection with server
        close(sockfd);
    }

    return 0;
}