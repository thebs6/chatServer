#include "chatserver.hpp"
#include <muduo/net/EventLoop.h>

int main() {
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8989);
    ChatServer server(&loop, addr, "server");

    server.start();
    loop.loop();

    return 0;
}