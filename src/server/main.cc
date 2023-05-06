#include "chatserver.hpp"
#include "chatservice.hpp"
#include <muduo/net/EventLoop.h>
#include <signal.h>

void resetHandler(int) {
    ChatService::instance()->reset();
    exit(0);
}

int main() {

    signal(SIGINT, resetHandler);
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8989);
    ChatServer server(&loop, addr, "server");

    server.start();
    loop.loop();

    return 0;
}