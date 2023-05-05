#pragma once

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
using namespace muduo;
using namespace muduo::net;

class ChatServer {
public:
    ChatServer(EventLoop *loop, const InetAddress& listenAddr, const string& nameArg);
    void start();
private:
    void onConncetion(const TcpConnectionPtr&);

    void onMessage(const TcpConnectionPtr&, Buffer*, Timestamp);

    TcpServer server_;
    EventLoop* loop_;
};