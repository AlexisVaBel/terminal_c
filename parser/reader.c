#include "reader.h"
#include "./../cmn/failures.h"
#include <stdio.h>

#define RDR_BUFFER_SIZE 1024
#define RDR_OK_STATUS 1
#define RDR_BAD_STATUS 0

char *kBufferPnt = 0;

int init_read_buffer(){
      kBufferPnt = malloc(sizeof(char) * RDR_BUFFER_SIZE);
      if(!kBufferPnt)
          return RDR_BAD_STATUS;
      return RDR_OK_STATUS;
}

void destr_read_buffer(){
    printf("killing pnt");
    free (kBufferPnt);
}


char *read_line_by_char(){
    int  ch=0;
    int  pos = 0;
    if(!kBufferPnt)emit_err("RDR buffer not created");
    while(1){
        ch = getchar();
        if (ch == EOF || ch == '\n') {
            kBufferPnt[pos] = '\0';
            return kBufferPnt;
        } else {
            kBufferPnt[pos] = ch;
        }
        pos++;
        if(pos>RDR_BUFFER_SIZE)
            emit_err("RDR buffer exceeded 1024 bytes");
    }
}

char *read_line(){
    char *line = NULL;
    ssize_t buffsize =0;
    getline(&line,&buffsize,stdin);
    return line;
}

char  *read_full_command(){
    return read_line_by_char();
}
