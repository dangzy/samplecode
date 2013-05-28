#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <cstdio>
#include <cstring>

struct msg_buf{
    long mtype;
    char data[255];
};

int main(){
    key_t mykey=ftok("/home",0);
    int queid=msgget(mykey,IPC_CREAT|0666);
    if(-1==queid){
        printf("msgget failed\n");
        return -1;
    }
    msg_buf mymsg;
    mymsg.mtype=1;
    strcpy(mymsg.data,"haha");
    return msgsnd(queid,&mymsg,strlen(mymsg.data)+1,0);
}
