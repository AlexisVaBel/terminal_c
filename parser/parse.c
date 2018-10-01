#include "parse.h"

#include "string.h"

const int kBufferSize = 128;

char ** commandList(char * ch, const char *chDelim){

    char *token      = ch;   
    size_t idx	     = 0;

    char **tokens = malloc((sizeof(char*))*kBufferSize);


    token  = strtok(ch,chDelim);
    while(token!=NULL){                
        (*(tokens + idx++))=token;
        token = strtok(NULL,chDelim);
    };
    (*(tokens + idx))=NULL;
    return tokens;
}
