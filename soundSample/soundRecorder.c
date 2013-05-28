#include<unistd.h>
#include<fcntl.h>
#include<sys/soundcard.h>
#include<stdio.h>
#include"common.h"


unsigned char buf[BUFFERLEN];

int main(){
    int fd;
    int arg;
    int status;

    fd=open("/dev/dsp",O_RDWR);
    if(fd<0){
        perror("open of /dev/dsp failed");
        exit(1);
    }

    arg=SIZE;
    status=ioctl(fd,SOUND_PCM_WRITE_BITS,&arg);
    if(status==-1)
        perror("SOUND_PCM_WRITE_BITS ioctl failed");
    if(arg!=SIZE)
        perror("unable to set sample size");
    
    arg=CHANNELS;
    status=ioctl(fd,SOUND_PCM_WRITE_CHANNELS,&arg);
    if(status=-1)
        perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
    if(arg!=CHANNELS)
        perror("unable to set number of channels");

    arg=RATE;
    status=ioctl(fd,SOUND_PCM_WRITE_RATE,&arg);
    if(status==-1)
        perror("SOUND_PCM_WRITE_RATE ioctl failed");

    printf("say something:\n");
    status=read(fd,buf,sizeof(buf));
    if(status!=sizeof(buf))
        perror("read wrong number of bytes");
    printf("You said:\n");
    status=write(fd,buf,sizeof(buf));
    if(status!=sizeof(buf))
        perror("write wrong number of bytes");
    status=ioctl(fd,SOUND_PCM_SYNC,0);
    if(status==-1)
        perror("SOUND_PCM_SYNC ioctl failed");

    close(fd);

    int filefd=open(SOUNDFILE,O_WRONLY|O_CREAT,666);
    write(filefd,buf,sizeof(buf));
    close(filefd);
    return 0;
}
