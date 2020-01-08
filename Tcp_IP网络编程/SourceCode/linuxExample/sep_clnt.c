#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 30
void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;
    FILE *readfp;
    FILE *writefp;
    if(argc != 3)
    {
        printf("Usage: %s <IP><Port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == -1)
            error_handling("socket() error");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_adr,sizeof(serv_adr)) == -1)
            error_handling("connect() error");
    else
            puts("connected...");

    readfp = fdopen(sock,"r");
    writefp = fdopen(sock,"w");
    while(1)
    {
        if(fgets(message,sizeof(message),readfp) == NULL)
                break;
        fputs(message,stdout);
        fflush(stdout);
    }
    fputs("From Client: thank you ！\n",writefp);
    fflush(writefp);
    fclose(readfp);
    fclose(writefp);
    return 0;
}
