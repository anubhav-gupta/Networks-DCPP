#include <iostream>
#include <cmath>
#include <sys/types.h>
#include <sys/wait.h>
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
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
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
#define PORT_NUMBER     5010
#define SERVER_ADDRESS  "127.0.0.1"
#define FILENAME        "foo.c"
#define FILE_TO_SEND    "hello.c"
#define RECIEVEPORTNUMBER 5000
int client_socket;
ssize_t len_client;
struct sockaddr_in remote_addr;
char buffer[BUFSIZ];
FILE *ClientFile;
int remain_data_client = 0;
int server_socket;
int peer_socket;
socklen_t   sock_len;
ssize_t len_server;
struct sockaddr_in      server_addr;
struct sockaddr_in      peer_addr;
int ServerFile;
int sent_bytes = 0;
int file_size;
struct stat file_stat;
off_t offset;
char send_data[1024];
int remain_data_server;
#include "Headers.cpp"
#include "Functions.cpp"
#include "ParserPeer1.cpp"
bool Allowed;
void GetFileStats()
{
	fstat(ServerFile, &file_stat);
}
string s;
void TranferFile(int size)
{
        offset = 0;
	while(((sent_bytes = sendfile(peer_socket,ServerFile,&offset,1024))>0)&&(size>0))
	{
		fprintf(stdout, "1. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, size);
		size -= sent_bytes;
		fprintf(stdout, "2. Server sent %d bytes from file's data, offset is now : %d and remaining data = %d\n", sent_bytes, offset, size);

	}
}
void DownloadFile(string s)
{
	ReadFileToSend(s.c_str());
	if(ServerFile==-1)
	{
		cout<<"Error Opening File\n";
	}
	GetFileStats();
	sprintf(send_data, "%d", file_stat.st_size);
	len_server = send(peer_socket,send_data,strlen(send_data),0);
	TranferFile(file_stat.st_size);
	close(ServerFile);

}
char message[8];
char Alert[12];
void RecieveFile()
{
	string str1;
	file_size = atoi(buffer);
	cout<<"File Size : "<<file_size<<endl;
	str1 = "foo.c";
	ReadFileToRecieve(str1.c_str());
	int count = 1024;/*
	while(file_size>0)		
	{
		len_client = recv(client_socket,buffer,1024,0);
		fwrite(buffer, sizeof(char), len_client, ClientFile);
		fprintf(stdout, "Receive %d bytes and we hope :- %d bytes\n", len_client, file_size);
		!file_size -= len_client;
		if(file_size<1024)
			count = file_size;
	}*/
	cout<<"Complete"<<endl;
	fclose(ClientFile);

}
int main()
{
    int status;
    ConnectToClient();
    ConnectToServer();
    string str;
    char rough[12];
    message[7] = '\0';
    Alert[11] = '\0';

    strcpy(Alert,"Not Allowed");
    strcpy(message,"Allowed");
    pid_t p = fork();	
    if(p==0)
    {
	    while(1)
	    {
		cout<<"Client"<<endl;
		gets(send_data);
		str = string(send_data);
		if(str.substr(0,16)=="FileUpload Allow")
		{
			FILE* fp;
			fp = fopen("permission","w");
			fprintf(fp,"%s",message);
			fclose(fp);
		}
		else if(str.substr(0,15)=="FileUpload Deny")
		{
			FILE* fp;
			fp = fopen("permission","w");
			fprintf(fp,"%s",Alert);
			fclose(fp);
		}
		else 
		{
			cout<<send_data<<endl;
			send(client_socket,send_data,strlen(send_data),0);
			if(str.substr(0,12)=="FileDownload")
			{
				getDownload(&client_socket,(str.substr(13,str.length() -12).c_str()));
			}
			else if(str.substr(0,10)=="FileUpload")
			{
				len_client = recv(client_socket,buffer,1024,0);
				buffer[client_socket] = '\0';
				if(!strcmp(buffer,"Not Allowed"))
				{
					cout<<"File Upload Not Allowed\n";
				}
				else
				{
					download(&client_socket,(str.substr(11,str.length()-10)).c_str());
				}
			}
			else 
			{
				getDownload(&client_socket,"heya");
				FILE* fp = fopen("heya","r");
				while(!feof(fp))
				{
					fscanf(fp,"%s",rough);
					cout<<rough<<endl;
				}
			}
		}
		/*******Getting The File Details*****/
	    }
	    _exit(1);
    }
    else
    {
	    while(1)
	    {
		 file_upload = false;
		 file_download= false;
		 cout<<"Server"<<endl;   
		 len_server = recv(peer_socket,buffer,1024,0);
		 buffer[len_server] = '\0';
		 str = string(buffer);
		 Parse(str);
		 if(file_download)
		 {
		 	s = str.substr(13,str.length()- 13 + 1);
			download(&peer_socket,s.c_str());	
			cout<<s<<endl;
		 }
		 else if(file_upload)
		 {
			 FILE* fp;
			 fp = fopen("permission","r"); 
			 fscanf(fp,"%s",rough);
			 fclose(fp);
			 if(!strcmp(rough,"Allowed"))
			 {
				send(peer_socket,message,strlen(message),0);
				getDownload(&peer_socket,str.substr(11,str.length()-10).substr().c_str());
			//	len_server = recv(peer_socket,buffer,1024,0);
			//	buffer[len_server] = '\0';
			//	cout<<buffer<<endl;
				cout<<"Complete"<<endl;
			 }
			 else
			 {
				send(peer_socket,Alert,strlen(Alert),0); 
			 }
		 }
		 else 
		 {
			 GetFile(peer_socket,in);
		 }
	    }
	    waitpid(p,&status,0);
    }
    return 0;
}

