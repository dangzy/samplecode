#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile int done = 0;

void handler (int sig)
{
    const char *str = "handled...\n";
    write (1, str, strlen(str));
    //done = 1;
}

void child(void)
{
    int i;
    for (i = 0; i < 3; i++){
        kill(getppid(), SIGRTMIN);
        printf("child - BANG!\n");
    }
    exit (0);
}

int main (int argc, char *argv[])
{
    signal (SIGRTMIN, handler);
    sigset_t newset, oldset;

    sigfillset(&newset);
    sigprocmask(SIG_BLOCK, &newset, &oldset);

    pid_t pid = fork();
    if (pid == 0)
        child();

    printf("parent sleeping \n");

    int r = sleep(30);

    printf("woke up! r=%d\n", r);

    sigprocmask(SIG_SETMASK, &oldset, NULL);

    while (!done){
    };

    printf("exiting\n");
    exit(0);
}
