#include "chatserver.hpp"
#include "chatservice.hpp"
#include <muduo/net/EventLoop.h>
#include <signal.h>

void resetHandler(int) {
    ChatService::instance()->reset();
    exit(0);
}

int main(int argc, char **argv) {

    char *ip = argv[1];
    uint16_t port = atoi(argv[2]);

    signal(SIGINT, resetHandler);
    EventLoop loop;
    InetAddress addr(ip, port);
    ChatServer server(&loop, addr, "server");

    server.start();
    loop.loop();

    return 0;
}