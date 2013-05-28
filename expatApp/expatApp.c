#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<expat.h>

#define XMLFILEPATH "expatApp.xml"
#define MAX_SERVICE_NUM 13
#define MAX_DAEMON_NUM 4

typedef struct expatService{
    char *name;
    char *cfgfile;
    int daemoncnt;
    char **daemon;
    char *launch;
}expatService;

typedef struct expatApp{
    int cnt;
    expatService *srv;
}expatApp;

char *pat[]={"cfgfile","daemon","launch",NULL};
char *currpat=NULL;

static void XMLCALL
startElement(void *data, const char *name, const char**atts){
    expatApp *app=data;
    int i=0;
    if(!strcmp(name,"service")){
        while(atts[i]){
            if(!strcmp(atts[i],"name")){
                app->srv[app->cnt].name=strdup(atts[i+1]);
                app->srv[app->cnt].daemoncnt=0;
                app->srv[app->cnt].daemon=malloc(MAX_DAEMON_NUM*sizeof(char*));
                break;
            }
            i+=2;
        }
        currpat=NULL;
    }
    else{
        while(pat[i]){
            if (!strcmp(name,pat[i])){
                currpat=pat[i];
                break;
            }
            i++;
        }
        if(!pat[i])
            currpat=NULL;
    }

}

static void XMLCALL
endElement(void *data,const char *name){
    expatApp *app=data;
    if(!strcmp(name,"service")){
        app->cnt++;
    }
}

static void XMLCALL
charactorData(void *data, const char *s, int len){
    expatApp *app=data;
    char buff[256]={0};
    strncpy(buff,s,len);
    if (!currpat)
        return;
    if (!strcmp(currpat,"cfgfile")){
        app->srv[app->cnt].cfgfile=strdup(buff);
    }else if(!strcmp(currpat,"daemon")){
        app->srv[app->cnt].daemon[app->srv[app->cnt].daemoncnt]=strdup(buff);
        app->srv[app->cnt].daemoncnt++;
    }else if(!strcmp(currpat,"launch")){
        app->srv[app->cnt].launch=strdup(buff);
    }
    currpat=NULL;
}

static void PrintInfo(expatApp *app){
    int i,j;
    for(i=0;i<app->cnt;i++){
        printf("==========================\n");
        printf("name=%s\n",app->srv[i].name);
        printf("cfgfile=%s\n",app->srv[i].cfgfile);
        for(j=0;j<app->srv[i].daemoncnt;j++)
            printf("daemon=%s\n",app->srv[i].daemon[j]);
        printf("launch=%s\n",app->srv[i].launch);
    }
}

int main(){
    XML_Parser parser=XML_ParserCreate(NULL);
    expatApp app;
    app.cnt=0;
    app.srv=malloc(MAX_SERVICE_NUM*sizeof(expatService));
    XML_SetUserData(parser,&app);
    XML_SetElementHandler(parser,startElement,endElement);
    XML_SetCharacterDataHandler(parser,charactorData);
    FILE *cfg=fopen(XMLFILEPATH,"r");
    char buff[256]={0};
    while(fgets(buff,sizeof(buff),cfg)){
        if ((XML_Parse(parser,buff,strlen(buff),0)) == XML_STATUS_ERROR){
            printf("%s at line %d\n",XML_ErrorString(XML_GetErrorCode(parser)),XML_GetCurrentLineNumber(parser));
            return 1;
        }
    }
    XML_Parse(parser,buff,0,1);
    PrintInfo(&app);
    XML_ParserFree(parser);
    return 0;
}
