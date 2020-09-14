#include "sctpserver.h"
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>

SctpServer::SctpServer()
    :streamIncrement_(0)
{

}

void SctpServer::listenSocket(void)
{
    //创建SCTP套接字
    if (IPV6_ENABLE) {
        sockFd_ = socket(AF_INET6,SOCK_SEQPACKET,IPPROTO_SCTP);
        bzero(&serverAddr_,sizeof(serverAddr_));
        serverAddr_.sin_family = AF_INET6;
        serverAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr_.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET6,"0.0.0.0",&serverAddr_.sin_addr);   

        //地址绑定
        bind(sockFd_,(struct sockaddr *)&serverAddr_,sizeof(serverAddr_));

        //设置SCTP通知事件(此处只设置了I/O通知事件)
        bzero(&events_,sizeof(events_));
        events_.sctp_data_io_event = 1;
        setsockopt(sockFd_,IPPROTO_SCTP,SCTP_EVENTS,&events_,sizeof(events_));

        //开始监听
        listen(sockFd_,LISTEN_QUEUE);
    } else {
        sockFd_ = socket(AF_INET6,SOCK_SEQPACKET,IPPROTO_SCTP);
        bzero(&serverAddr_,sizeof(serverAddr_));
        serverAddr_.sin_family = AF_INET;
        serverAddr_.sin_addr.s_addr = htonl(INADDR_ANY);
        serverAddr_.sin_port = htons(SERVER_PORT);
        inet_pton(AF_INET,"0.0.0.0",&serverAddr_.sin_addr);   

        //地址绑定
        bind(sockFd_,(struct sockaddr *)&serverAddr_,sizeof(serverAddr_));

        //设置SCTP通知事件(此处只设置了I/O通知事件)
        bzero(&events_,sizeof(events_));
        events_.sctp_data_io_event = 1;
        setsockopt(sockFd_,IPPROTO_SCTP,SCTP_EVENTS,&events_,sizeof(events_));

        //开始监听
        listen(sockFd_,LISTEN_QUEUE);
    }
}

void SctpServer::loop(void)
{
    while(true)
    {
        len_ = sizeof(struct sockaddr_in);
        //从socket读取内容
        readSize_ = sctp_recvmsg(sockFd_,readBuf_,BUFFER_SIZE,
                                 (struct sockaddr *)&clientAddr_,&len_,&sri_,&messageFlags_);
								 
		printf("Server received: %ld Bytes \n", readSize_);
        //增长消息流号
        if(streamIncrement_)
        {
            sri_.sinfo_stream++;
        }
        sctp_sendmsg(sockFd_,readBuf_,readSize_,
                     (struct sockaddr *)&clientAddr_,len_,
                      sri_.sinfo_ppid,sri_.sinfo_flags,sri_.sinfo_stream,0,0);
    }
}

int SctpServer::recv(void*buf)
{
    len_ = sizeof(struct sockaddr_in);
        //从socket读取内容
    readSize_ = sctp_recvmsg(sockFd_,readBuf_,BUFFER_SIZE,
                                 (struct sockaddr *)&clientAddr_,&len_,&sri_,&messageFlags_);
								 
	printf("Server received: %ld Bytes \n", readSize_);

    //decode the sctp message

    //
    //tmp_buf = (unsigned char*)malloc(readSize_);

    memcpy(buf,readBuf_,readSize_);

    return readSize_;
}
void SctpServer::send(void *buf, int size)
{
    printf("Cgo SctpServer send size: %d ", size);
    char *temp = (char*)buf;
    for (int i = 0; i < size; i++){
        printf("%X ", *temp);
        temp++;
    }
        sctp_sendmsg(sockFd_,buf,size,
                     (struct sockaddr *)&clientAddr_,len_,
                      sri_.sinfo_ppid,sri_.sinfo_flags,sri_.sinfo_stream,0,0);
}

void SctpServer::start(void)
{
    listenSocket();
    //loop();
}
