#include "comprocessor.h"

#include <termios.h>
#include <dirent.h>
#include <sys/types.h>



#include <unistd.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <sys/ioctl.h>
#include <sys/signal.h>

const int kPortProcCode     = 1;
const int kPortNameCode     = 2;
const int kPortBaudCode     = 3;
const int kPortParityCode   = 4;
const int kPortStopBitsCode = 5;



const char *kCOMDir         = "/dev";

int iportID = 0;

// functions for work in builtIn commands
void read_port(char ** pntChCmds){
    char *chPack[5];
    int iVal=(read(iportID,chPack,5));
    return(iVal>0);
}

void write_port(char ** pntChCmds){
    tcflush(iportID,TCIOFLUSH);
    return (write(iportID,"hello",5)>0);
}

void port_list(char ** pntChCmds){
    DIR *dir;
    struct dirent *dp;
    dir=opendir(kCOMDir);

    if(dir==0)return;
    if(dp != NULL)printf(dp->d_name);
    while((dp=readdir(dir))!=NULL){
        if(sizeof(dp->d_name)>3){
            if((dp->d_name[0]=='t')&&(dp->d_name[1]=='t')&&(dp->d_name[2]=='y')){
                printf("port: %s\n",dp->d_name);
            }
        }
    }
    closedir(dir);
}

void port_connect(char ** pntChCmds){
    printf("connect\n");
    int iSucs = 0;

    int iBaud = B9600;
    int iDataBits = CS8;
    int iStopBits = CSTOPB;
    int iFlow     = 0;

    char *chNameport = malloc(strlen(kCOMDir)+strlen(pntChCmds[kPortNameCode])+1);
    strcpy(chNameport, kCOMDir);
    strcat(chNameport, pntChCmds[kPortNameCode]);
    iportID=open(chNameport,O_RDWR|O_NOCTTY|O_NDELAY);
    printf("port id %d\n",iportID);
    printf("port name %s\n",chNameport);
    //
    struct termios newtio;
    tcgetattr(iportID, &newtio);
// clear old conf
    newtio.c_cflag&=~(CS5|CS6|CS7|CS8);
    newtio.c_cflag&=~(PARENB|PARODD);
    newtio.c_cflag&=~CSTOPB;
    // setting read write paramters
    newtio.c_lflag &=~OPOST;          // - не выполнять обработку вывода
    newtio.c_lflag &=~ECHO;           // - запретить эхо-вывод введенных символов
//    newtio.c_lflag &=~ECHOE;        //  запретить стираие последнего символа в строке
    // (Если разрешить и включен канонический режим, то производится стирание посл. символа)
    newtio.c_lflag &=~ISIG;              // - запретить генерацию сигналов, при приеме символов.
    newtio.c_lflag &=~ICANON;      // - октлючить канонический режим (теперь читаем по-символьно)
// clear old conf
    cfsetispeed(&newtio,iBaud);
    cfsetospeed(&newtio,iBaud);
    newtio.c_cflag|=iDataBits;
    newtio.c_cflag|=iStopBits;
    newtio.c_cflag|=iFlow;
// with handler symbols
//    struct sigaction saio;
//    saio.sa_handler=signal_handler_IO;
//    saio.sa_flags=0;
//    saio.sa_restorer=NULL;
//    sigaction(SIGIO,&saio,NULL);
//    fcntl(m_iPortID,F_SETOWN,getpid()); //  allow process to receive SIGIO
//    fcntl(m_iPortID,F_SETFL,FASYNC);   // make procs async
// with handler symbols
    newtio.c_lflag             |=  CLOCAL;                // игнорировать строки состояния модема
    newtio.c_lflag             |=  CREAD;                   //  разрешить прием
    newtio.c_cc[VMIN]   =1;
    newtio.c_cc[VTIME] =1;
    tcflush(iportID,TCIOFLUSH);
    iSucs=tcsetattr(iportID,TCSANOW,&newtio);           // установка сейчас
    free(chNameport);
    return (iSucs==0);
}

void port_disconnect(){
    printf("port id %d\n",iportID);
    if(iportID!=(0)){
        printf("closing port numer %d\n",iportID);
        iportID=close(iportID);
    }
    printf("result port id %d\n",iportID);
}

void port_help(char ** pntChCmds){
    printf("print help for this help\n");
    printf("print list to see all ports\n");
    printf("print \"connect portname baud parity stopbits\" to connect to port \n");
}

// and here can go on

char *port_cmds[]={
  "list",
  "connect",
  "read",
  "write",
  "disconnect",
  "help",
};

int (*port_funct[]) (char **) = {
        &port_list,
        &port_connect,
        &read_port,
        &write_port,
        &port_disconnect,
        &port_help
        };

int port_funct_num(){
    return sizeof(port_cmds)/sizeof(char*);
}




void cmds_ports(char ** pntChCmds){
    int i=0;
    for (i = 0; i < port_funct_num(); i++) {
        if(pntChCmds[kPortProcCode]==NULL){
            return;
        }
        if (strcmp(pntChCmds[kPortProcCode], port_cmds[i]) == 0) {
            return (*port_funct[i])(pntChCmds);
        }
    };
}

