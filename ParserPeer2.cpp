#include<cstdio>
#include<iostream>
#include<string>
#include<string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/stat.h>
FILE *in;
char buff[512];
using namespace std;
string SHARED="/home/anubhav/stud";
FILE* LongList();
int ShortList(string start,string end);
int SendData(char *send_data);
int RegEx(string filename);
void GenereateHash(string reg);
void HashAll();
void Parse(string str)
{
	string r,t,u,v,s;
	string start,end;
	string filename;
	int len;
	s = str.substr(0,8);
	if(s=="IndexGet")
	{
		cout<<"Found IndexGet"<<endl;
		r = str.substr(9,8);
		t = str.substr(9,9);
		u = str.substr(9,5);
		if(r=="LongList")
		{
			cout<<"heh"<<endl;
			in = LongList();
			//LongList();
		}
		else if(t=="ShortList")
		{
			cout<<"Found Shortlist"<<endl;
			start=str.substr(19,10);
			end=str.substr(30,10);
			cout<<start<<endl;
			cout<<end<<endl;
			ShortList(start,end);
		}
		else if(u=="RegEx")
		{
			len=str.length() -13;
			filename=str.substr(14,len);
			cout<<"filename"<<endl;
			RegEx(filename);
		//	RegEx(); I will do this tommorrow it's a 10 minute job
		}
	}
	else if(s=="FileHash")
	{
		r=str.substr(9,6);
		t=str.substr(9,9);
		if(r=="Verify")
		{
			len=str.length()-15;
			t=str.substr(16,len);
			GenereateHash(t);
		}
		if(t=="Check All")
		{
			HashAll();
		}

	}
	else 
	{
		s = str.substr(0,11);
		if(s=="FileDownload")
		{
			file_download = true;
		}
		else if(s=="FileUpload ")
		{
			file_upload = true;
		}
		else
		{
			cout<<str<<endl;
		}
	}
}
string getstamp(string path)
{
	struct stat attr;
	stat(path.c_str(), &attr);
	string temp = ctime(&attr.st_mtime);
	return temp;
}
void ParsetoString()
{
	cout<<"Kya ho gaya"<<endl;
	char to_send[100000];
	fgets(buff, sizeof(buff), in);
	cout<<buff<<endl;
	strcpy(to_send,buff);
	while(fgets(buff, sizeof(buff), in)!=NULL)
	{
		strcat (to_send,buff);
	}
	cout << to_send <<endl;
	int len=strlen(to_send);
	to_send[len]='\0';
	pclose(in);
	SendData(to_send);
}
void HashAll()
{
	string to_execute="md5sum "+SHARED+"/*";
	if(!(in = popen(to_execute.c_str(), "r")))
	{
//		return 1;
	}
	//ParsetoString();
}
void GenereateHash(string reg)
{
	string to_execute="md5sum "+SHARED+"/"+reg;
	if(!(in = popen(to_execute.c_str(), "r")))
	{
//		return 1;
	}
//	ParsetoString();
}
int  RegEx(string filename)
{
	string to_execute="ls -al "+SHARED+"|grep "+filename;
	if(!(in = popen(to_execute.c_str(), "r")))
	{
		return 1;
	}
//	ParsetoString();
}
int ShortList(string start,string end)
{
	char to_send[100000];
	string to_execute="touch -d '" + start + "' " +SHARED+"/"+start;
	string to_execute2="touch -d '" + end + "' " +SHARED+"/"+end;
	system(to_execute.c_str());
	system(to_execute2.c_str());
	string a="find "+SHARED+"/"+" -type f -anewer "+SHARED+"/"+start+" ! -anewer "+SHARED+"/"+end+" 2> /dev/null";
	printf("%s\n",a.c_str());
	if(!(in = popen(a.c_str(), "r")))
	{
		return 1;
	}
/*	to_execute="rm "+SHARED+"/"+start;
	to_execute2="rm "+SHARED+"/"+end;
	system(to_execute.c_str());
	system(to_execute2.c_str());	
*/
//	ParsetoString();
}
int SendData(char *send_data)
{
	int sock, bytes_recieved;  
	char recv_data[1024];
	struct hostent *host;
	struct sockaddr_in server_addr;  

	host = gethostbyname("127.0.0.1");

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("Socket");
		exit(1);
	}

	server_addr.sin_family = AF_INET;     
	server_addr.sin_port = htons(5000);   
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8); 

	if (connect(sock, (struct sockaddr *)&server_addr,
				sizeof(struct sockaddr)) == -1) 
	{
		perror("Connect");
		exit(1);
	}

//	while(1)
	{

/*		bytes_recieved=recv(sock,recv_data,1024,0);
		recv_data[bytes_recieved] = '\0';

		if (strcmp(recv_data , "q") == 0 || strcmp(recv_data , "Q") == 0)
		{
			close(sock);
			break;
		}

		else
			printf("\nRecieved data = %s " , recv_data);
*/
		printf("\nSEND (q or Q to quit) : ");

		if (strcmp(send_data , "q") != 0 && strcmp(send_data , "Q") != 0)
		{
			send(sock,send_data,strlen(send_data), 0); 
		}
		else
		{
			send(sock,send_data,strlen(send_data), 0);   
			close(sock);
		}
	}   
}
FILE* LongList()
{
	char to_send[100000];
	string to_execute="ls -al "+SHARED;
	if(!(in = popen(to_execute.c_str(), "r")))
	{
//		return 1;
	}
	return in;
	//ParsetoString();
}
string GiveHash(string reg)
{
	char hash[36];
	string to_execute="md5sum "+SHARED+"/"+reg;
	if(!(in = popen(to_execute.c_str(), "r")))
	{
		// return 1;
	}
	fgets(hash,sizeof(hash),in);
	hash[strlen(hash)]='\0';
	string t = string(hash);
	return t;
}
