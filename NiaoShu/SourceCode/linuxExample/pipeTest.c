#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE 30
int main()
{
    int fds[2];
    pid_t pid;
    int i,str_len;
    char buf[BUF_SIZE];

    pipe(fds);
    pid = fork();
    if(pid == 0)
    {
        for(i=0 ; i < 3; ++i)
        {
            write(fds[1],"child",5);
            sleep(2);
            str_len = read(fds[0],buf,BUF_SIZE);
            buf[str_len] = 0;
            printf("child receive message: %s\n",buf);
        }
        return 0;
    }
    else
    {
        for(i = 0; i < 3; ++i)
        {
            str_len = read(fds[0],buf,BUF_SIZE);
            buf[str_len] = 0;
            printf("Parent receive message: %s\n",buf);
            write(fds[1],"Parent",6);
            sleep(2);
        }
    }
    return 0;


}
