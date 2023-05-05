#include "chatservice.hpp"
#include "public.hpp"
#include <muduo/base/Logging.h>
#include <vector>

using namespace std;
using namespace muduo;

using namespace std::placeholders;

ChatService* ChatService::instance() {
    static ChatService service;
    return &service;
}

ChatService::ChatService() {
    msgHandlerMap_.insert({ EnMsgType::LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3) });
}

void ChatService::login(const TcpConnectionPtr& conn, json& json, Timestamp timestamp) {
    LOG_INFO << "login";
}

void ChatService::reg(const TcpConnectionPtr& conn, json& json, Timestamp timestamp) {
    LOG_INFO << "reg";
}


auto ChatService::getHandler(int msgid) -> MsgHandler {
    if(msgHandlerMap_.find(msgid) == msgHandlerMap_.end()) {
        return [&msgid](const TcpConnectionPtr&, json&, Timestamp) {
            LOG_ERROR << "msgid:" << msgid << "can not find handler!";
        };
    }

    return msgHandlerMap_[msgid];
}