#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
void error_handling(char *message)
{
     fputs(message,stderr);
     fputc('\n',stderr);
     exit(1);
}

int main(int argc,char *argv[])
{
    int send_sock;
    struct sockaddr_in adr;
    int so_brd = 1;

    if(argc != 3)
    {
        printf("Usage: %s <Broadcast IP><PORT>\n",argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(send_sock == -1)
            error_handling("socket() error");

    memset(&adr,0,sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = inet_addr(argv[1]);
    adr.sin_port = htonl(atoi(argv[2]));
    setsockopt(send_sock,SOL_SOCKET,SO_BROADCAST,(void*)&so_brd,sizeof(so_brd));
    sendto(send_sock,"hujinrun",strlen("hujinrun"),0,(struct sockaddr*)&adr,sizeof(adr));
    close(send_sock);
    return 0;
}