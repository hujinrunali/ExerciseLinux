#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void error_handling(char*message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char*argv[])
{
    int sock;
    char message1[] = "hujinrun";
    char message2[] = "love";
    char message3[] = "liyanjing";
    struct sockaddr_in serv_adr;

    if(argc != 3)
    {
        printf("Usage: %s <IP><PORT>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if(sock == -1)
            error_handling("socket() error");

    memset(&serv_adr,0,sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    sendto(sock,message1,strlen(message1),0,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
    sendto(sock,message2,strlen(message2),0,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
    sendto(sock,message3,strlen(message3),0,(struct sockaddr*)&serv_adr,sizeof(serv_adr));
     
    close(sock);
    return 0;
}
