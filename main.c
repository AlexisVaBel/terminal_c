#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <signal.h>
#include <errno.h>
#include "./process/execute.h"
#include "./parser/reader.h"
#include "./parser/parse.h"

#define MAIN_MSG_HELLO "<<"
#define MAIN_MSG_FAULT "\n[!] Terminal_ Segmentation fault\n"

void loop();

void segfault_handler(int signo){
    fprintf(stderr,MAIN_MSG_FAULT);
    sleep(10);
    printf(errno);
    exit (-2);
}

int main(void){
    signal(SIGSEGV,segfault_handler);
    loop();
    return EXIT_SUCCESS;
}


void loop(){
    int status = init_read_buffer();

    char *cmdLine;        

    while(status){
        printf(MAIN_MSG_HELLO);
        cmdLine = read_full_command();
        //
        status = process(commandList(cmdLine," "));
        //
        if(strcmp(cmdLine, "quit") == 0) status=0;

    };
    destr_read_buffer();
}

