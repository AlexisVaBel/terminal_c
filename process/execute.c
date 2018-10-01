#include "execute.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include <termios.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/signal.h>
#include <sys/types.h>

const int kContinueCode = 1;
const int kBreakCode    = 0;

// functions for work in buildin commands
int term_help(char ** pntChCmds);
int term_exit(char ** pntChCmds);
int term_comports(char ** pntChCmds);
// and here can go on
//

char *term_cmds[]={
  "help",
  "exit",
  "coms"
};

int (*term_funct[]) (char **) = {
        &term_help,
        &term_exit,
        &term_comports
        };

int term_funct_num(){
    return sizeof(term_cmds)/sizeof(char*);
}


int term_help(char **pntChCmds){
    int i;
    printf("Author Alex Bel\n");
    printf("Terminal help\n");
    for(i=0;i < term_funct_num();i++){
        printf("%s\n",term_cmds[i]);
    }
    return kContinueCode;
}

int term_exit(char ** pntChCmds){
    return kBreakCode;
}

int term_comports(char ** pntChCmds){
    DIR *dir;
    struct dirent *dp;
    dir=opendir("/dev");

    if(dir==0)return kBreakCode;
    while((dp=readdir(dir))!=NULL){
        if(sizeof(dp->d_name)>3){
            if((dp->d_name[0]=='t')&&(dp->d_name[1]=='t')&&(dp->d_name[2]=='y')){
                printf("port: %s\n",dp->d_name);
            }
        }
    }
    closedir(dir);
    return kContinueCode;
}

int process(char ** pntChCmds){
    int i=0;
    int iResult = kContinueCode;
    if(*pntChCmds == NULL)return kBreakCode;

    for (i = 0; i < term_funct_num(); i++) {
        if (strcmp(pntChCmds[0], term_cmds[i]) == 0) {
            iResult=(*term_funct[i])(pntChCmds);
            break;
        }
    }

    free(pntChCmds);
    return iResult;
};

