#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define TTL 64
#define BUF_SIZE 30
void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char*argv[])
{
    int send_sock;
    struct sockaddr_in mul_addr;
    int time_live = TTL;
    char buf[BUF_SIZE];
    if(argc != 3)
    {
        printf("Usage: %s <GroupIP><Port>\n",argv[0]);
        exit(1);
    }

    send_sock = socket(PF_INET,SOCK_DGRAM,0);
    if(send_sock == -1)
            error_handling("socket() error");

    memset(&mul_addr,0,sizeof(mul_addr));
    mul_addr.sin_family = AF_INET;
    mul_addr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock,IPPROTO_IP,IP_MULTICAST_TTL,(void*)&time_live,sizeof(time_live));
    sendto(send_sock,"hujinrun",strlen("hujinrun"),0,(struct sockaddr*)&mul_addr,sizeof(mul_addr));
    close(send_sock);
    return 0;
}
