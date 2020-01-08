#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int serv_sock,clnt_sock;
    char message[BUF_SIZE];
    int str_len,i;

    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t adr_sz;
    FILE *readfp;
    FILE *writefp;

    if(argc != 2)
    {
        printf("Usage: %s <IP><PORT>\n",argv[0]);
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
    readfp = fdopen(clnt_sock,"r");
    writefp = fdopen(dup(clnt_sock),"w");
    fputs("From server:hello\n",writefp);
    fputs("hujinrun",writefp);
    fflush(writefp);
   
    shutdown(fileno(writefp),SHUT_WR);
    fclose(writefp);
    fgets(message,BUF_SIZE,readfp);
    fputs(message,stdout); 
    fclose(readfp);
    close(serv_sock);
    return 0;
}
