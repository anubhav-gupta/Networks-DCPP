
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sendfile.h>
int main(int argc,char **argv)
{
	int src;
	int dest;
	struct stat stat_buf;
	off_t offset = 0;
	src = open(argv[1],O_RDONLY);
	fstat(src,&stat_buf);
	dest = open(argv[2],O_WRONLY|O_CREAT, stat_buf.st_mode);
	long long remain_data;
	int len;
	remain_data = stat_buf.st_mode;
	printf("%d\n",remain_data);
	while(len = sendfile(dest,src,&offset,1024)>0&&stat_buf.st_mode>0)
	{
		printf("%d\n",len);
		sendfile(dest,src,&offset,stat_buf.st_size);
		stat_buf.st_mode-=len;
	}
}
