#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OPTZ 4
int calculate(int opnum,int opnds[],char operator)
{
    int result=opnds[0],i;
    switch(operator)
    {
        case '+':
                for(i = 1;i < opnum;++i)
                        result += opnds[i];
                break;
        case '-':
                for(i = 1;i < opnum;++i)
                        result -= opnds[i];
                break;
        case '*':
                for(i = 1;i < opnum;++i)
                        result *= opnds[i];
                break;
        case '/':
                for(i = 1;i < opnum;++i)
                        result /= opnds[i];
                break;
        default:
                break;
    }
    return result;
}


void error_handling(char *message)
{
    fputs(message,stderr);
    fputc('\n',stderr);
    exit(1);
}

int main(int argc,char *argv[])
{
    int serv_sock,clnt_sock;
    struct sockaddr_in serv_addr,clnt_addr;
    socklen_t clnt_addr_len;
    char opinfo[BUF_SIZE];
    int recv_cnt,recv_len;
    int result,i;
    char opnd_cnt;

    if(argc != 2)
    {
        printf("Usage: %s <port>",argv[0]);
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

    clnt_addr_len = sizeof(clnt_addr);
    for(i = 0; i < 5;++i)
    {
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_len);
        if(clnt_sock == -1)
                error_handling("accept() error");
        else
                printf("Connected client : %d\n",i+1);

        
        read(clnt_sock,&opnd_cnt,1);
        printf("opnd_cnt: %d\n",opnd_cnt);
        recv_cnt = 0;
        while(recv_cnt < (OPTZ*opnd_cnt+1))
        {
            recv_len = read(clnt_sock,&opinfo[recv_cnt],BUF_SIZE-1);
            recv_cnt += recv_len;
            printf("recv_cnt: %d",recv_cnt);
        }
        result = calculate(opnd_cnt,(int *)opinfo,opinfo[recv_cnt-1]);
        printf("The result is : %d\n",result);
        write(clnt_sock,(char *)&result,sizeof(result));
        close(clnt_sock);
    }
    close(serv_sock);
    return 0;
}
