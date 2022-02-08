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


int compute_expression(char input[100])
{
    int len_inp = strlen(input), i, currNum = 0, ans = 0;

    
    char ls_op = '+'; 

    input[len_inp] = '+';    
    input[++len_inp] = '\0'; 
    len_inp++;

    for (i = 0; i < len_inp; i++)
    {
        
        if (input[i] >= '0' && input[i] <= '9')
            currNum = currNum * 10 + input[i] - '0';
        else
        {
            // We have the first number and can perform the op on ans
            switch (ls_op)
            {
            case '+':
                ans += currNum;
                break;
            case '-':
                ans -= currNum;
                break;
            case '*':
                ans *= currNum;
                break;
            case '/':
                ans /= currNum;
                break;
            default:
                return -1;
            }
            currNum = 0;     
            ls_op = input[i]; 
        }
    }
    return ans;
}

int main()
{
    char buff[100];                              
    int sockfd, newSockfd;                       
    unsigned int port = 58040;                   
    struct sockaddr_in server_addr, client_addr; 
    int client_len;
    int opt = 1;
    // Get a stream socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        printf("Error opening socket\n");
        return 1;
    }

    // Put server information into server structure
    server_addr.sin_family = AF_INET;
    // The port must be put into network byte order
    server_addr.sin_port = htons(port);
    // Address for server is '127.0.0.1' so INADDR_ANY is used
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Binds this process to the port
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Error binding to port %d\n", port);
        return 1;
    }
    printf("Connected to PORT: %d\n", port);

    
    listen(sockfd, 5);

    while (1)
    {
        client_len = sizeof(client_addr);
        
        newSockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);
        if (newSockfd < 0)
        {
            printf("Accept error\n");
            return 1;
        }

        
        if (recv(newSockfd, buff, 100, 0) < 0)
        {
            printf("Error receiving data\n");
            return 1;
        }
        printf("Received: %s\n", buff);

        int ans = compute_expression(buff);
        
        bzero(buff, 100);
        
        snprintf(buff, sizeof(buff), "%d", ans);

        
        if (send(newSockfd, buff, 100, 0) < 0)
        {
            printf("Error sending data\n");
            return 1;
        }

        
        bzero(buff, 100);
        
        close(newSockfd);
    }

    return 0;
}