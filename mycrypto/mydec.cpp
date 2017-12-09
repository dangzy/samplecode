#include <iostream>
#include <fstream>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>
using namespace std;

int main(int argc, char** argv) {
    if(argc!=2) {
        cout << "param num error"<<endl;
        return argc;
    }
    char cmd[1024] = {0};
    char tmpxz[1024] = {0};
    char middlefile[1024] = {0};
    time_t tmpname = time(NULL);
    sprintf(tmpxz,"/tmp/%ld.xz",tmpname);
    sprintf(cmd,"dd if=%s of=%s skip=16 bs=1",argv[1],tmpxz);
    system(cmd);
    sprintf(cmd,"xz -d %s",tmpxz);
    system(cmd);
    sprintf(middlefile,"%ld",tmpname);
    sprintf(cmd,"objdump -x /tmp/%ld|grep .encdata|awk '{system(\"od -N 0x\"$3\" -j 0x\"$6\" -t x1 -A none -w1 -v /tmp/%ld\")}'|awk '{printf(\"%%02x\",strtonum(\"0x\"$1))}' > %s",tmpname,tmpname,middlefile);
    system(cmd);
    ifstream infile(middlefile);
    char orgfile[1024] = {0};
    sprintf(orgfile,"%ld.org",tmpname);
    ofstream outfile(orgfile,ios_base::binary|ios_base::out);
    char mychar;
    unsigned char orgchar = 0;
    do {
        mychar = 0;
        infile.get(mychar);
        if(!mychar)
            break;
        if(mychar>='0'&&mychar<='9')
            orgchar = (mychar-'0')<<4;
        else
            orgchar = (mychar-'a'+0xa)<<4;
        mychar = 0;
        infile.get(mychar);
        if(!mychar)
            break;
        if(mychar>='0'&&mychar<='9')
            orgchar += (mychar-'0');
        else
            orgchar += (mychar-'a'+0xa);
        outfile.put((orgchar^0xff)&0xff);
    }while(true);
    infile.close();
    outfile.close();
    sprintf(cmd,"rm -rf %s /tmp/%s",middlefile,middlefile);
    system(cmd);
    return 0;
}
