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
    int queid=msgget(mykey,0666);
    if(-1==queid){
        printf("msgget failed\n");
        return -1;
    }
    msg_buf mymsg;
    msgrcv(queid,&mymsg,sizeof(mymsg.data),0,0);
    printf("%s\n",mymsg.data);
    return 0;
}
