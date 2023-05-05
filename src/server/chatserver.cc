#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <iostream>
#include <functional>
#include <string>
using namespace std;
using namespace placeholders;
using json = nlohmann::json;

ChatServer::ChatServer(EventLoop *loop, const InetAddress& listenAddr, const string& nameArg)
    : server_(loop, listenAddr, nameArg), loop_(loop) {
    server_.setConnectionCallback(std::bind(&ChatServer::onConncetion, this, _1));

    server_.setMessageCallback(std::bind(&ChatServer::onMessage, this, _1, _2, _3));

    server_.setThreadNum(4);
}
void ChatServer::start() {
    server_.start();
}

void ChatServer::onConncetion(const TcpConnectionPtr& conn) {
    if(!conn->connected()) {
        conn->shutdown();
    }
}

void ChatServer::onMessage(const TcpConnectionPtr& conn, Buffer* buffer, Timestamp timestamp) {
    string buf = buffer->retrieveAllAsString();
    std::cout << buf << std::endl;

    json js = json::parse(buf);

    std::cout << js["msgid"] << std::endl;

    auto msg_handler = ChatService::instance()->getHandler(js["msgid"].get<int>());

    msg_handler(conn, js, timestamp);
}