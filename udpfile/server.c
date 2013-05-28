#include<stdio.h>
#include<netinet/in.h>
#include<string.h>
#include"mycommon.h"

int main(){
    struct sockaddr_in servaddr,cliaddr;
    socklen_t cliaddr_len;
    int sockfd=socket(AF_INET,SOCK_DGRAM,0);
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family=AF_INET;
    servaddr.sin_addr.s_addr=htonl(INADDR_ANY);
    servaddr.sin_port=htons(SERV_PORT);
    bind(sockfd,(struct sockaddr*)&servaddr,sizeof(servaddr));
    
    FILE *tranfile=fopen(TRANSFERFILE,"r");
    char buff[MAXLEN]={0};
    while(1){
	cliaddr_len=sizeof(cliaddr);
	recvfrom(sockfd,buff,MAXLEN,0,(struct sockaddr*)&cliaddr,&cliaddr_len);
	if(!strcmp(STARTCMD,buff))
	    break;
    }
    while(fgets(buff,sizeof(buff),tranfile))
	sendto(sockfd,buff,strlen(buff),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
    sendto(sockfd,ENDCMD,strlen(ENDCMD),0,(struct sockaddr*)&cliaddr,sizeof(cliaddr));
    return 0;
}
