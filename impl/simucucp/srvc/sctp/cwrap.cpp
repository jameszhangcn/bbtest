#include "cwrap.h"
#include "sctpserver.h"
static SctpServer *global_sctpserver;

void startSctp() {
    global_sctpserver = new(SctpServer);
    global_sctpserver->start();
}

int recvSctp(void* buf){
   return global_sctpserver->recv(buf);
}

int sendSctp(void* buf, int size){
    global_sctpserver->send(buf,size);
}
