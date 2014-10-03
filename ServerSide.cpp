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
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
#include <unistd.h>
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
#define PORT_NUMBER 5000
#define SERVER_ADDRESS "127.0.0.1"
#define FILE_TO_SEND "h.c"
const double PI=3.14159265358979323846264338327950288419716939937510582097494459230;
void swaps (char *x,char *y){char temp;temp=*x;*x=*y;*y=temp;}
void swapi(int *a,int *b){int temp;temp=*a;*a=*b;*b=temp;}
ULL gcd(ULL a,ULL b){if(a==0)return b;if(b==0)return a;if(a==1||b==1)return 1;
if(a==b)return a;if(a>b)return gcd(b,a%b);else return gcd(a,b%a);}

int Server_Socket;
int peer_socket;
socklen_t   sock_len;
ssize_t len;
struct sockaddr_in      Server_addr;
struct sockaddr_in      peer_addr;
FILE* fd;
int sent_bytes = 0;
char file_size[256];
struct stat file_stat;
off_t offset;
int remain_data;
ifstream file;
void ServerSocketError()
{
	fprintf(stderr, "Error creating socket --> %s", strerror(errno));
	exit(EXIT_FAILURE);
}
int GetFileSize()
{
	file.seekg(0, std::ifstream::end);
	int val = file.tellg();
	return val;
}
void ReadAndSend()
{
	/*file.open("hello.c");
	string s;
	int len_file = GetFileSize();
	file.clear();
	file.seekg(0,ios::beg);
	stringstream ss;
	ss<<len_file;
	ss>>s;*/
	fd = fopen("hello.c","r");
	char *s1 = new char[1024];
	int num = 0;
	char c;
	cout<<"In"<<endl;
        //len = send(peer_socket, s.c_str(),s.length() , 0);
	while(fscanf(fd,"%c",&c)!=EOF)
	{
		num = 0;
		s1[num++] = c;
		while(num<1024&&fscanf(fd,"%c",&c)!=EOF)
		{
			s1[num++] = c;
		}
		send(peer_socket,s1,num,0);
	}
	cout<<"he"<<endl;
	//file.close();
}
int main()
{

    Server_Socket = socket(AF_INET, SOCK_STREAM, 0);
    if(Server_Socket == -1)
    {
	ServerSocketError();
    }
    memset(&Server_addr,0,sizeof(Server_addr));
    Server_addr.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_ADDRESS, &(Server_addr.sin_addr));
    Server_addr.sin_port = htons(PORT_NUMBER);
    if((bind(Server_Socket, (struct sockaddr *)&Server_addr, sizeof(struct sockaddr))) == -1)
    {
	    fprintf(stderr, "Error on bind --> %s", strerror(errno));
	    exit(EXIT_FAILURE);
    }
    if ((listen(Server_Socket, 5)) == -1)   {
	    fprintf(stderr, "Error on listen --> %s", strerror(errno));

	    exit(EXIT_FAILURE);
    }

    sock_len = sizeof(struct sockaddr_in);
    peer_socket = accept(Server_Socket, (struct sockaddr *)&peer_addr, &sock_len);
    if (peer_socket == -1){
	    fprintf(stderr, "Error on accept --> %s", strerror(errno));
	    exit(EXIT_FAILURE);
    }
    //fprintf(stdout, "Accept peer --> %s\n", inet_ntoa(peer_addr.sin_addr));
    ReadAndSend();
 	   close(peer_socket);         
    close(Server_Socket);
    return 0;
}

