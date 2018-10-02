#include "execute.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


const int kOKCode = 1;
const int kBreakCode    = 0;

// functions for work in builtIn commands
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
    printf("==========Author Alex Bel==========\n");
    printf("========== Terminal help ==========\n");
    for(i=0;i < term_funct_num();i++){
        printf("%s\n",term_cmds[i]);
    }
    return kOKCode;
}

int term_exit(char ** pntChCmds){
    return kBreakCode;
}

int term_comports(char ** pntChCmds){
    cmds_ports(pntChCmds);
    return kOKCode;
}

int process(char ** pntChCmds){
    int i=0;
    int iResult = kOKCode;
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

