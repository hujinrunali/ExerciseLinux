#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#define BUF_SIZE 30
int acpt_sock;
int recv_sock;

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}


void urg_handler(int signo);
int main(int argc,char *argv[])
{
    struct sockaddr_in acpt_addr,recv_addr;
    int str_len,state;
    socklen_t adr_sz;
    struct sigaction act;
    char buf[BUF_SIZE];

    if(argc != 2)
    {
        printf("Usage : %s <Port> \n",argv[0]);
        exit(1);
    }

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET,SOCK_STREAM,0);
    if(acpt_sock == -1)
            error_handling("socket() error");

    memset(&acpt_addr,0,sizeof(acpt_addr));
    acpt_addr.sin_family = AF_INET;
    acpt_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_addr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock,(struct sockaddr*)&acpt_addr,sizeof(acpt_addr)) == -1)
            error_handling("bind() error");

    if(listen(acpt_sock,5) == -1)
            error_handling("listen() error");

    adr_sz = sizeof(recv_addr);
    recv_sock = accept(acpt_sock,(struct sockaddr*)&recv_addr,&adr_sz);

    fcntl(recv_sock,F_SETOWN,getpid());
    state = sigaction(SIGURG,&act,0);

    while((str_len = recv(recv_sock,buf,sizeof(buf)-1,0)) != 0)
    {
        if(str_len == -1)
                continue;
        buf[str_len] = 0;
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);
    return 0;
    
}

void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock,buf,sizeof(buf)-1,MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s\n",buf);
}
