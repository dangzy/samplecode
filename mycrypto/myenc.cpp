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
    struct stat infilestat;
    if (0!=stat(argv[1],&infilestat)){
        cout << "input file not found" <<endl;
        return argc;
    }
    char filename[1024] = {0};
    sprintf(filename,"/tmp/%ld.cpp",time(NULL));
    ofstream outfile(filename,ios_base::out|ios_base::binary);
    ifstream infile(argv[1],ios_base::in|ios_base::binary);
    char inbuffer[1024];
    memset(inbuffer,0,sizeof(inbuffer));
    sprintf(inbuffer,
            "#include <iostream>\n\
            using namespace std;\n\
            const unsigned char mydata[%ld] __attribute__ ((section (\".encdata\"))) = {\n",
            infilestat.st_size);
    outfile << inbuffer;
    MD5_CTX c;
    MD5_Init(&c);
    streamsize insize = 0;
    char rawdata[10] = {0};
    streamsize total = 0;
    char inchar;
    while(total < infilestat.st_size) {
        inchar = 0;
        infile.get(inchar);
        total++;
        MD5_Update(&c, &inchar, sizeof(inchar));
        inchar ^= 0xff;
        sprintf(rawdata,"0x%02x, ", inchar&0xff);
        outfile << rawdata;
    };
    outfile.seekp(-2,ios_base::cur);
    outfile << "};\n\
        class helloworld{\n\
            public:\n\
                   helloworld(){}\n\
                       virtual ~helloworld(){}\n\
                       void sayhelloworld(){cout << __func__ <<endl;}\n\
        };\n\
    int main() {\n\
        helloworld *hw = new helloworld;\n\
            hw->sayhelloworld();\n\
            delete hw;\n\
    }";
    outfile.flush();
    char cmd[1024] = {0};
    sprintf(cmd,"g++ %s -o %s.enc -s",filename,argv[1]);
    system(cmd);
    sprintf(cmd,"xz -z %s.enc",argv[1]);
    system(cmd);
    unsigned char inmd5[17] = {0};
    MD5_Final(inmd5, &c);
    char finalfilepath[1024];
    sprintf(finalfilepath,"%s.txt",argv[1]);
    ofstream finalfile(finalfilepath,ios_base::out|ios_base::binary);
    finalfile << inmd5;
    finalfile.close();
    sprintf(cmd, "cat %s.enc.xz >> %s.txt",argv[1],argv[1]);
    system(cmd);
    infile.close();
    outfile.close();
    sprintf(cmd,"rm -rf %s %s.enc.xz",filename,argv[1]);
    system(cmd);
    return 0;
}
