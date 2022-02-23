/*
    NAME: RIDAM HAZRA
	ROLL: 19CS8040
*/

// Server side implementation of UDP Client-Server Model

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

// For directory and file operations
#include <sys/stat.h>
#include <dirent.h>

#define PORT 8040
#define MAXINBUF 256
#define MAXOUTBUF 1024

// Driver code

int main() {
	int sockfd;
	char inBuffer[MAXINBUF], outBuffer[MAXOUTBUF];

	struct sockaddr_in server_addr, client_addr;
	
	struct stat sb;
	struct dirent *de;
	
	// For creating socket file descriptor
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	memset(&client_addr, 0, sizeof(client_addr));
	
	// For filling server information
	server_addr.sin_family = AF_INET; // IPv4
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);
	
	// For binding the socket with the server address
	if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) 
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	
	int client_len, n;
	
	printf("Connected to PORT: %d\n", PORT);

	listen(sockfd, 5);
	
	while(1)
	{
		client_len = sizeof(client_addr);
		if ((n = recvfrom(sockfd, inBuffer, MAXINBUF, MSG_WAITALL, (struct sockaddr *)&client_addr, &client_len)) < 0) 
		{
            printf("Error receiving data\n");
            return 1;
        }

		inBuffer[n] = '\0';

		// If directory exists
		if (stat(inBuffer, &sb) == 0 && S_ISDIR(sb.st_mode)) 
		{
			DIR *dir = opendir(inBuffer);
			if (dir == NULL) 
			{
				printf("Could not open directory");
				return 0;
			}

			// Checking all contents of the directory
			// And appending them to output buffer
			while (((de = readdir(dir)) != NULL) && strlen(outBuffer) < 1024) 
			{
				strcat(outBuffer, de->d_name);
				strcat(outBuffer, "\\0");
			}

			strcat(outBuffer, "\0");
			
			if (sendto(sockfd, outBuffer, MAXOUTBUF, MSG_CONFIRM, (const struct sockaddr *) &client_addr, client_len) < 0) 
			{
				printf("Error sending data\n");
				return 1;
			}
		}
		else 
		{ 	// If directory does not exist
			outBuffer[0] = '\0';
			if (sendto(sockfd, outBuffer, MAXOUTBUF, MSG_CONFIRM, (const struct sockaddr *) &client_addr, client_len) < 0) 
			{
				printf("Error sending data\n");
				return 1;
			}
		}

		// Clear the buffers
		bzero(outBuffer, MAXOUTBUF);
		bzero(inBuffer, MAXINBUF);
	}

	bzero(outBuffer, MAXOUTBUF);
	bzero(inBuffer, MAXINBUF);

	return 0;
}