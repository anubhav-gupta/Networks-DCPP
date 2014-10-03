#include <iostream>
#include <cmath>
#include <algorithm>
#include <limits>
#include <vector>
#include <bitset>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <time.h>
#include <map>
#include <stack>
#include <string>
#include <climits>
#include <ctime>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
using namespace std;
bool file_download;
bool file_upload;
void ConnectToServer()
{
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

}
void ConnectToClient()
{
        //int remain_data;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1)
        {
                fprintf(stderr, "Error creating socket --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Zeroing server_addr struct */
        memset(&server_addr, 0, sizeof(server_addr));
        /* Construct server_addr struct */
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, SERVER_ADDRESS, &(server_addr.sin_addr));
        server_addr.sin_port = htons(RECIEVEPORTNUMBER);

        /* Bind */
        if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) == -1)
        {
                fprintf(stderr, "Error on bind --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }

        /* Listening to incoming connections */
        if ((listen(server_socket, 5)) == -1)
        {
                fprintf(stderr, "Error on listen --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
	sock_len = sizeof(struct sockaddr_in);
	peer_socket = accept(server_socket, (struct sockaddr *)&peer_addr, &sock_len);
        if (peer_socket == -1)
        {
                fprintf(stderr, "Error on accept --> %s", strerror(errno));

                exit(EXIT_FAILURE);
        }
        fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));

}
void ReadFileToSend(char const *name)
{
	 ServerFile = open(name, O_RDONLY);
}
void ReadFileToRecieve(char const *name)
{
	ClientFile = fopen(name,"w");
}


