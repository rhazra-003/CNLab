/*
  	NAME: RIDAM HAZRA
	ROLL: 19CS8040
*/

// Client side implementation of UDP Client-Server Model

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8040
#define MAXINBUF 1024
#define MAXOUTBUF 256

// Driver code

int main() {
	int sockfd;
	char inBuffer[MAXINBUF], outBuffer[MAXOUTBUF], temp[MAXINBUF];
	char *hello = "Hello from client";
	struct sockaddr_in server_addr;

	// For creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));
	
	// For filling server information
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = INADDR_ANY;
	
	int n, server_len;

	// Iterators
	int i = 0, j = 0;

	while(1) 
	{
		printf("Enter the path to the folder: ");
		fgets(outBuffer, MAXOUTBUF, stdin);
        outBuffer[strcspn(outBuffer, "\n")] = '\0';
		if (strcmp(outBuffer, "-1") == 0) 
		{
            return 0;
        }

		if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
		{
            printf("Connection Failed\n");
            return 1;
        }

		if (sendto(sockfd, outBuffer, MAXOUTBUF, MSG_CONFIRM, (const struct sockaddr *) &server_addr, server_len) == -1) 
		{
            printf("Error in sending data\n");
            return 1;
        }

		if ((n = recvfrom(sockfd, inBuffer, MAXINBUF, MSG_WAITALL, (struct sockaddr *)&server_addr, &server_len)) < 0)
		{
            printf("Error receiving data\n");
            return 1;
        }

		// If empty string sent, the directory does not exist.
		
		if (inBuffer[0] == '\0') 
		{
			printf("Directory does not exist!\n");
			return 0;
		}
		
		// For printing the different directories and Files present in the directory
		
		printf("The contents of %s are:\n", outBuffer);
        while (i < n && j < n) 
		{
			if (inBuffer[j] == '\\') 
			{
				strncpy(temp, inBuffer+i, j-i);
				temp[j-i+1] = '\0';
			
				printf("%s\n", temp);
			
				j += 2;
				i = j;
				bzero(temp, MAXINBUF);
			}
			
			else 
			{
				++j;
			}
		}

        bzero(inBuffer, MAXINBUF);
		bzero(outBuffer, MAXOUTBUF);
		close(sockfd);
		
		return 0;
	}

	close(sockfd);
	
	return 0;
}