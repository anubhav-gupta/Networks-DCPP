/* Client code */
/* TODO : Modify to meet your need */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT_NUMBER     5000
#define SERVER_ADDRESS  "127.0.0.1"
#define FILENAME        "foo.c"
#define RECEIVEPORTNUMBER 5010
int client_socket;
ssize_t len;
struct sockaddr_in remote_addr;
char buffer[BUFSIZ];
int file_size;
FILE *received_file;
int remain_data = 0;
int main(int argc, char **argv)
{
//	ConnectToServer();
        /* Zeroing remote_addr struct */
        memset(&remote_addr, 0, sizeof(remote_addr));

        /* Construct remote_addr struct */
        remote_addr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_ADDRESS, &(remote_addr.sin_addr));
        remote_addr.sin_port = htons(PORT_NUMBER);

        /* Create client socket */
        client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1)
        {
                fprintf(stderr, "Error creating socket --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Connect to the server */
        if (connect(client_socket, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr)) == -1)
        {
                fprintf(stderr, "Error on connect --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Receiving file size */
        int bytes_received = recv(client_socket, buffer, 1024, 0);
	buffer[bytes_received] = '\0';
	printf("%s\n",buffer);
        file_size = atoi(buffer);
        //fprintf(stdout, "\nFile size : %d\n", file_size);

        received_file = fopen("foo.c", "w");
        if (received_file == NULL)
        {
                fprintf(stderr, "Failed to open file foo --> %s\n", strerror(errno));

                exit(EXIT_FAILURE);
        }
        remain_data = file_size;
        while (((len = recv(client_socket, buffer, 1024, 0)) > 0) && (remain_data > 0))
        {
        	fwrite(buffer, sizeof(char), len, received_file);
                fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
        	remain_data -= len;
        }
        fclose(received_file);
        close(client_socket);

        return 0;
}
