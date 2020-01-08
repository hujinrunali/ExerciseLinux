#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OPTZ 4
#define RS_SIZE 4

void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char optmsg[BUF_SIZE];
    int i,op_cnt,result;

    if(argc != 3)
    {
        printf("Usage: %s <IP><PORT>\n",argv[0]);
        exit(1);
    }


    sock = socket(PF_INET,SOCK_STREAM,0);
    if(sock == -1)
        error_handling("socket() error");

    //连接服务器
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr)) == -1)
              error_handling("connect() error");

    fputs("Operand count:",stdout);
    scanf("%d",&op_cnt);
    optmsg[0] = (char)op_cnt;
    for(i = 0;i < op_cnt; i++)
    {
        printf("Operand %d: ",i+1);
        scanf("%d",(int *)&optmsg[i*OPTZ+1]);
    }
    fgetc(stdin);
    fputs("Operator: ",stdout);
    scanf("%c",&optmsg[op_cnt*OPTZ+1]);
    write(sock,optmsg,op_cnt*OPTZ+2);
    read(sock,&result,RS_SIZE);
    printf("The result is : %d\n",result);
    close(sock);
    return 0;
}
