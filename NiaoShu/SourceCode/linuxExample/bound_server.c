#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char*message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char*argv[])
{
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in host_adr,from_adr;
    socklen_t adr_len;
    int str_len,i;

    if(argc != 2)
    {
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if(sock == -1)
            error_handling("socket() error");

    memset(&host_adr,0,sizeof(host_adr));
    host_adr.sin_family = AF_INET;
    host_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    host_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sock,(struct sockaddr *)&host_adr,sizeof(host_adr)) == -1)
            error_handling("bind() error");

    for(i = 0;i<3;++i)
    {
        adr_len = sizeof(from_adr);
        str_len =recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&from_adr,&adr_len);
        message[str_len] = 0;
        printf("Message from client: %s\n",message);
    }
    close(sock);
    return 0;
}
