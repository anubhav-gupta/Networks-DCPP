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
/* Client code */
/* TODO : Modify to meet your need */
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>

#define PORT_NUMBER     5000
#define SERVER_ADDRESS  "127.0.0.1"
#define FILENAME        "foo.c"
using namespace std;
#define LL long long
#define ULL unsigned long long
#define LD long double
#define MAX(a,b) ((a)>(b)?(a):(b))
#define MIN(a,b) ((a)<(b)?(a):(b))
#define ABS(x) ((x)<0?-(x):(x))
#define si(n) scanf("%d",&n)
#define sf(n) scanf("%f",&n)
#define ss(n) scanf("%s",n)
#define pnl printf("\n")
#define REP(i,n) for(int i=0;i<(n);i++)
#define FOR(i,a,b) for(int i=(a);i<(b);i++)
#define FORR(i,n) for(int i=(n);i>=0;i--)
#define DB(x) cout<<"\n"<<#x<<" = "<<(x)<<"\n";
#define CL(a,b) memset(a,b,sizeof(a))
#define GOLD ((1+sqrt(5))/2)
const double PI=3.14159265358979323846264338327950288419716939937510582097494459230;
void swaps (char *x,char *y){char temp;temp=*x;*x=*y;*y=temp;}
void swapi(int *a,int *b){int temp;temp=*a;*a=*b;*b=temp;}
ULL gcd(ULL a,ULL b){if(a==0)return b;if(b==0)return a;if(a==1||b==1)return 1;
if(a==b)return a;if(a>b)return gcd(b,a%b);else return gcd(a,b%a);}
int client_socket;
ssize_t len;
struct sockaddr_in remote_addr;
char buffer[1030];
int file_size;
FILE *received_file;
int remain_data = 0;
void ReadAndWrite()
{
	ofstream file;
	string str;
	received_file = fopen("foo.c","a+");
	//file.open("foo.c",ios::out | ios::app);
	//num = recv(client_socket,)
	len = recv(client_socket,buffer,1024,0);
	buffer[len] = '\0';
	 while (!strcmp("Complete",buffer))
	  {
		  FOR(i,0,len)
			  fprintf(received_file,"%c",buffer[i]);
		 // buffer[len] = '\0';
		 /* str = string(buffer);
		  file<<str;*/
		  len = recv(client_socket,buffer,1024,0);
		  fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len, remain_data);
		  //remain_data -= len;

	  }

}
int main()
{
     
     
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
      ReadAndWrite();
     close(client_socket);

    return 0;
}

