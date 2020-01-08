#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdlib.h>
void sigint(int sig)
{
    char message[10];
    fputs("Input Y to stop: ",stdout);
    fgets(message,10,stdin);
    if(strcmp(message,"Y\n") == 0)
            exit(1);
    else
            fputs("Continue Process",stdout);
}

int main()
{
    struct sigaction act;
    act.sa_handler = sigint;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGINT,&act,0);

    while(1)
    {
        puts("wait...");
        sleep(10);
    }
    return 0;
}
