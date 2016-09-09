#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/user.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
    pid_t traced_process;
    struct user_regs_struct regs;
    long ins;
    if(argc != 2) {
        printf("Usage: %s <pid to be traced>\n",argv[0]);
        return 1;
    }
    traced_process = atoi(argv[1]);
    printf("we are going to trace: %d\n",traced_process);
    ptrace(PTRACE_ATTACH, traced_process,
            NULL, NULL);
    //getchar();
    //ptrace(PTRACE_SYSCALL,traced_process, NULL, NULL);
    //wait(NULL);
    ptrace(PTRACE_GETREGS, traced_process, NULL, &regs);
    //ptrace(PTRACE_CONT, traced_process, NULL, NULL);
    ptrace(PTRACE_DETACH,traced_process,NULL,NULL);
    printf("pc:%llx\n",regs.rip);

    void *mmap_addr, *dlopen_addr, *dlsym_addr, *dlclose_addr;
    
    ptrace(PTRACE_DETACH, traced_process,
            NULL, NULL);
    return 0;
}
