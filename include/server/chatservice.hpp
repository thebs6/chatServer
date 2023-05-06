#pragma once

#include <muduo/net/Callbacks.h>
#include <muduo/net/TcpConnection.h>
#include <unordered_map>
#include <functional>
#include <mutex>

#include "friendmodel.hpp"
#include "groupmodel.hpp"
#include "json.hpp"
#include "offlinemessagemodel.hpp"
#include "usermodel.hpp"

using namespace std;
using namespace muduo;
using namespace muduo::net;

using json = nlohmann::json;

using MsgHandler = std::function<void(const TcpConnectionPtr &conn, json &js, Timestamp)>;

class ChatService {
public:
    static ChatService* instance();
    void login(const TcpConnectionPtr& conn, json& js, Timestamp timestamp);

    void reg(const TcpConnectionPtr& conn, json& js, Timestamp timestamp);

    void oneChat(const TcpConnectionPtr& conn, json& js, Timestamp timestamp);

    void addFriend(const TcpConnectionPtr& conn, json& js, Timestamp timestamp);

    void createGroup(const TcpConnectionPtr& conn, json& js, Timestamp timestamp);

    void addGroup(const TcpConnectionPtr& conn, json& js, Timestamp timestamp);

    void groupChat(const TcpConnectionPtr& conn, json& js, Timestamp timestamp);



    auto getHandler(int msgid) -> MsgHandler;

    void clientCloseException(const TcpConnectionPtr& conn);

    void reset();
private:
    ChatService();

    std::unordered_map<int, MsgHandler> msgHandlerMap_;

    std::unordered_map<int, TcpConnectionPtr> user_conn_; 

    std::mutex conn_mutex_;

    UserModel user_model_;
    OfflineMsgModel offline_msg_model_;
    FriendModel friend_model_;
    GroupModel group_model_;
};