#include<stdio.h>
#include<string.h>
#include<netinet/in.h>
#include"mycommon.h"

int main(){
    struct sockaddr_in servaddr;
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&servaddr.sin_addr);
    servaddr.sin_port=htons(SERV_PORT);
    
    sendto(sockfd,STARTCMD,strlen(STARTCMD),0,(struct sockaddr*)&servaddr,sizeof(servaddr));
    while(1){
	char buff[MAXLEN]={0};
	recvfrom(sockfd,buff,MAXLEN,0,NULL,0);
	if(!(strcmp(buff,ENDCMD)))
	    break;
	printf("%s",buff);
	buff[0]=0;
    }
    return 0;
}
