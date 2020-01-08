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

int main(int argc ,char* argv[])
{
    int clnt_sock,serv_sock;
    struct sockaddr_in clnt_addr,serv_addr;
    socklen_t adr_sz;
    int str_len;
    char buf[BUF_SIZE];

    if(argc != 2)
    {
        printf("Usage : %s <Port>\n",argv[0]);
        exit(1);
    }
    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if(serv_sock == -1)
            error_handling("socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
            error_handling("bind() error");

    if(listen(serv_sock,5) == -1)
            error_handling("listen() error");

    adr_sz = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&adr_sz);
    if(clnt_sock == -1)
            error_handling("accept() error");

    while(1)
    {
        str_len = recv(clnt_sock,buf,BUF_SIZE,MSG_PEEK|MSG_DONTWAIT);
        if(str_len > 0)
                break;
    }

    buf[str_len] = 0;
    printf("Buffering %d bytes : %s \n",str_len,buf);

    str_len = recv(clnt_sock,buf,BUF_SIZE,0);
    buf[str_len] = 0;
    printf("read again: %s\n",buf);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}
