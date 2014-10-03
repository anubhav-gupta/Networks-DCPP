#define LENGTH 512
void GetFile(int,FILE *);
void RecieveFile(int,FILE *);
void getDownload(int *sock,const char *pch){

	int sockfd=*sock;
	FILE *fp;
	fp=fopen(pch,"wb");
	RecieveFile(sockfd,fp);
}
void RecieveFile(int sockfd,FILE *fp)
{
	char revbufd[LENGTH];
	bzero(revbufd,LENGTH);
	int fr_block_szd1 = 0;
	int fr_block_szd2 = 0;
	unsigned int size = 0;

	cout<<"1213"<<endl;
	while((fr_block_szd1 = recv(sockfd, &size, sizeof(size),0)) > 0)
	{
		//cout<<"213"<<endl;
		int packet_size=ntohl(size);
		fr_block_szd2 = recv(sockfd, revbufd, LENGTH, 0);
		if(fr_block_szd2 > 0) {

			int write_sz = fwrite(revbufd, sizeof(char), packet_size, fp);
		//	revbufd[write_sz]=0;
		//	printf("%s\n",revbufd);
			if(write_sz < packet_size)
			{
				//error_msg("[Client] File write failed on client.\n");
			}
			bzero(revbufd, LENGTH);
			if (packet_size == 0 || packet_size != LENGTH) 
			{
				break;
			}
		}
		if(fr_block_szd2 < 0)
		{
			if (errno == EAGAIN)
			{
				printf("recv() timed out.\n");
			}
			else
			{
				fprintf(stderr, "recv() failed due to errno = %d\n", errno);
				exit(1);
			}
		}
	}
	if(fr_block_szd1 < 0)
	{
		if (errno == EAGAIN)
		{
			printf("recv() timed out.\n");
		}
		else
		{
			fprintf(stderr, "recv() failed due to errno = %d\n", errno);
			exit(1);
		}
	}
	printf("Ok received from server!\n");

	fclose(fp);
	return;
}
void download(int* nsock,const char* file){

	int nsockfd=*nsock;
	printf("[Server] Sending %s file\n",file);
	FILE *fs=fopen(file,"rb");
	GetFile(nsockfd,fs);
}
void GetFile(int nsockfd,FILE* fs)
{
	//int file_size=fsize(file);
	//if(fs==NULL)
		//error_msg("[Server] File Not Found : ");
	int fs_block_sz;
	char sdbuf[LENGTH];
	int sdlen = 0;
	bzero(sdbuf,LENGTH);
	while(( fs_block_sz=fread(sdbuf,sizeof(char),LENGTH,fs) )>0){
		cout<<"Hehe"<<endl;
		unsigned int sdlen=htonl(fs_block_sz);

		if(send(nsockfd,&sdlen,sizeof(sdlen),0) < 0){

			fprintf(stderr, "[Server] ERROR: Failed to send header %d. (errno = %d)\n", fs_block_sz, errno);
			//error_msg("[Server] Sending Header Failed : ");
			break;

		}

		if(send(nsockfd, sdbuf, fs_block_sz, 0) < 0){

			fprintf(stderr, "[Server] ERROR: Failed to send file. (errno = %d)\n", errno);
			//error_msg("[Server] Sending File Data Failed : ");
			break;

		}
		bzero(sdbuf, LENGTH);

	}
	printf("Ok File  from Client was Sent!\n");
	return;
}

