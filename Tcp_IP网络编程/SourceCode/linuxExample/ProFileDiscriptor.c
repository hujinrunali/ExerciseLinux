#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

int main()
{
    int sock;
    pid_t pid;
    
    sock = socket(PF_INET,SOCK_STREAM,0);
    pid = fork();
    if(pid==0)
            printf("sock : %d\n",sock);
    else
            printf("sock: %d\n",sock);
    close(sock);
    return 0;
}
