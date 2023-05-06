#include "chatservice.hpp"
#include "offlinemessagemodel.hpp"
#include "public.hpp"
#include "user.hpp"
#include <muduo/base/Logging.h>
#include <mutex>
#include <vector>

using namespace std;
using namespace muduo;

using namespace std::placeholders;

ChatService* ChatService::instance() {
    static ChatService service;
    return &service;
}

void ChatService::reset() {
    user_model_.resetState();
}



ChatService::ChatService() {
    msgHandlerMap_.insert({ EnMsgType::LOGIN_MSG, std::bind(&ChatService::login, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::REG_MSG, std::bind(&ChatService::reg, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::ONE_CHAT_MSG, std::bind(&ChatService::oneChat, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::ADD_FRIEND_MSG, std::bind(&ChatService::addFriend, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::CREATE_GROUP_MSG, std::bind(&ChatService::createGroup, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::ADD_GROUP_MSG, std::bind(&ChatService::addGroup, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::GROUP_CHAT_MSG, std::bind(&ChatService::groupChat, this, _1, _2, _3) });
    msgHandlerMap_.insert({ EnMsgType::LOGINOUT_MSG, std::bind(&ChatService::loginOut, this, _1, _2, _3) });

    if(redis_.connect()) {
        redis_.init_notify_handler(std::bind(&ChatService::handleRedisSubscribeMessage, this, _1, _2));
    }
}

void ChatService::handleRedisSubscribeMessage(int userId, std::string msg) {
    std::lock_guard<std::mutex> lock(conn_mutex_);
    auto it = user_conn_.find(userId);
    if(it != user_conn_.end()) {
        it->second->send(msg);
    }

    offline_msg_model_.insert(userId, msg);
}

void ChatService::login(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {
    LOG_INFO << "login";
    int id = js["id"];
    std::string pwd = js["password"];

    User user = user_model_.query(id);
    json response;
    if(user.getId() != id || user.getPwd() != pwd) {
        response["msgid"] = LOGIN_MSG;
        response["errno"] = 1; 
        response["errmsg"] = "id or password is invalid!";
        conn->send(response.dump());
        return ;
    }

    if(user.getState() == UserState::ONLINE) {
        json response;
        response["msgid"] = LOGIN_MSG_ACK;
        response["errno"] = 2;
        response["errmsg"] = "this account is using, input another!";
        conn->send(response.dump());
        return;
    }

    {
        lock_guard<mutex> lock(conn_mutex_);
        user_conn_.insert({id, conn});
    }

    redis_.subscribe(id);

    user.setState(UserState::ONLINE);
    user_model_.updateState(user);

    response["msgid"] = LOGIN_MSG_ACK;
    response["errno"] = 0;
    response["id"] = user.getId();
    response["name"] = user.getName();

    auto offline_msg = offline_msg_model_.query(id);

    if(!offline_msg.empty()) {
        response["offlineMsg"] = offline_msg;
        offline_msg_model_.remove(id);
    }

    auto friends = friend_model_.query(id);
    if(!friends.empty()) {
        vector<string> friends_vec;
        for(auto& f : friends) {
            json js;
            js["id"] = f.getId();
            js["name"] = f.getName();
            js["state"] = UserStateToString(f.getState());
            friends_vec.emplace_back(js.dump());
        }
        response["friends"] = friends_vec;
    }

    conn->send(response.dump());
}

void ChatService::reg(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {
    LOG_INFO << "reg";
    std::string name = js["name"];
    std::string pwd = js["pwd"];
    User user;
    user.setName(name);
    user.setPwd(pwd);
    user.setState(UserState::OFFLINE);

    auto state = user_model_.insert(user);

    json response;

    if(state) {
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 0;
        response["id"] = user.getId();
    } else {
        response["msgid"] = REG_MSG_ACK;
        response["errno"] = 1;
    }

    conn->send(response.dump());
}

void ChatService::oneChat(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {
    
    int toid = js["toid"].get<int>();

    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        if(user_conn_.find(toid) != user_conn_.end()) {
            user_conn_[toid]->send(js.dump());
            return;
        }
    }

    User user = user_model_.query(toid);
    if(user.getState() == UserState::ONLINE) {
        redis_.publish(toid, js.dump());
    }

    offline_msg_model_.insert(toid, js.dump());
    
}

void ChatService::addFriend(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {
    int userid = js["id"].get<int>();
    int friendid = js["friendid"].get<int>();

    friend_model_.insert(userid, friendid);
}

auto ChatService::getHandler(int msgid) -> MsgHandler {
    if(msgHandlerMap_.find(msgid) == msgHandlerMap_.end()) {
        return [&msgid](const TcpConnectionPtr&, json&, Timestamp) {
            LOG_ERROR << "msgid:" << msgid << "can not find handler!";
        };
    }

    return msgHandlerMap_[msgid];
}


void ChatService::createGroup(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {

    int userid = js["id"].get<int>();
    std::string groupName = js["groupname"];
    std::string groupDesc = js["groupdesc"];

    Group group(-1, groupName, groupDesc);

    if (group_model_.createGroup(group)) {
        group_model_.addGroup(userid, group.getId(), GroupUserRoleToString(GroupUserRole::CREATOR));
    }
}

void ChatService::addGroup(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    group_model_.addGroup(userid, groupid, "normal");
}

void ChatService::groupChat(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();
    vector<int> useridVec = group_model_.queryGroupUsers(userid, groupid);

    lock_guard<mutex> lock(conn_mutex_);
    for (int id : useridVec) {
        auto it = user_conn_.find(id);
        if (it != user_conn_.end()) {
            // 转发群消息
            it->second->send(js.dump());
        } else {
            User user = user_model_.query(id);
            if(user.getState() == UserState::ONLINE) {
                redis_.publish(id, js.dump());
            }
            offline_msg_model_.insert(id, js.dump());
        }
    }
}

void ChatService::clientCloseException(const TcpConnectionPtr& conn) {
    User user;
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        for (const auto &item : user_conn_) {
            if (item.second == conn) {
                user.setId(item.first);
                user_conn_.erase(item.first);
                break;
            }
        }
    }

    redis_.unsubscribe(user.getId());

    if(user.getId() != -1) {
        user.setState(UserState::OFFLINE);
        user_model_.updateState(user);
    }
    
}


void ChatService::loginOut(const TcpConnectionPtr& conn, json& js, Timestamp timestamp) {
    int user_id = js["id"].get<int>();
    {
        std::lock_guard<std::mutex> lock(conn_mutex_);
        auto it = user_conn_.find(user_id);
        if(it != user_conn_.end()) {
            user_conn_.erase(it);
        }
    }

    redis_.unsubscribe(user_id);

    User user(user_id, "", "", UserState::OFFLINE);
    user_model_.updateState(user);
}