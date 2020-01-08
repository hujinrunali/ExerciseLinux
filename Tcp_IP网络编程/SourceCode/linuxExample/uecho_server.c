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
    int str_len;
    socklen_t clnt_adr_sz;
    struct sockaddr_in serv_addr,clnt_addr;

    if(argc != 2)
    {
        printf("Usage: %s <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if(sock ==-1)
            error_handling("UDP socket() error");

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
            error_handling("bind() error");

    while(1)
    {
        clnt_adr_sz = sizeof(clnt_addr);
        str_len = recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_addr,&clnt_adr_sz);
       message[str_len] = 0;
        printf("Message from client: %s\n",message);
        if(strcmp(message,"q\n") == 0)
                break;
        sendto(sock,message,str_len,0,(struct sockaddr*)&clnt_addr,clnt_adr_sz); 
    }
    close(sock);
    return 0;
}


