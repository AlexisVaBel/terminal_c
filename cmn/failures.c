#include "failures.h"
#include <stdio.h>
#include <stdlib.h>
void emit_err(char *errMsg){
    fprintf(stderr, errMsg);
    exit(EXIT_FAILURE);
}
