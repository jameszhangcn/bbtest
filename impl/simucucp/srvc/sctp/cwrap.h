#ifndef __CWARP_H__
#define __CWARP_H__
#ifdef __cplusplus

extern "C" {
#endif
void startSctp();

int recvSctp(void* buf);

int sendSctp(void* buf, int size);

#ifdef __cplusplus
}

#endif
#endif
