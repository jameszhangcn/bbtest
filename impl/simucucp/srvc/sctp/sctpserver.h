#ifndef SCTPSERVER_H
#define SCTPSERVER_H

#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>

#define SERVER_PORT 38462
#define BUFFER_SIZE 1024
#define LISTEN_QUEUE 100

#define IPV6_ENABLE true

class SctpServer
{
    public:
        SctpServer();
        void start(void);
        int recv(void* buf);
        void send(void *buf, int size);
    private:
        //开启监听socket
        void listenSocket(void);
        //循环处理请求
        void loop(void);

        int sockFd_;                            //用来接受的套接字
        int messageFlags_;                      //消息类型
        char readBuf_[BUFFER_SIZE];             //接受缓冲区
        struct sockaddr_in clientAddr_;         //用来保存客户端地址
        struct sockaddr_in serverAddr_;         //用来保存服务端地址
        struct sockaddr_in6 clientAddr6_;         //用来保存客户端地址
        struct sockaddr_in6 serverAddr6_;         //用来保存服务端地址
        struct sctp_sndrcvinfo sri_;            //消息相关细节信息
        struct sctp_event_subscribe events_;    //事件集
        int streamIncrement_;                   //流号
        socklen_t len_;                         //地址长度
        size_t readSize_;                       //读到的大小
};

void StartServer(void);

#endif
