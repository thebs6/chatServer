#pragma once

#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>

#include "json.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;

using json = nlohmann::json;

using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

class ChatService {
public:
    static ChatService* instance();
    void login(const TcpConnectionPtr& conn, json& json, Timestamp timestamp);
    void reg(const TcpConnectionPtr& conn, json& json, Timestamp timestamp);

    auto getHandler(int msgid) -> MsgHandler;
private:
    ChatService();

    std::unordered_map<int, MsgHandler> msgHandlerMap_;
};