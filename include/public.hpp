#pragma once

#include <string>
enum EnMsgType {
    LOGIN_MSG = 1,
    LOGIN_MSG_ACK,
    LOGINOUT_MSG,
    REG_MSG,
    REG_MSG_ACK,
    ONE_CHAT_MSG,
    ADD_FRIEND_MSG,

    CREATE_GROUP_MSG, 
    ADD_GROUP_MSG, 
    GROUP_CHAT_MSG, 
};

enum UserState {
    OFFLINE = 0,
    ONLINE = 1,
    INVALID = -1,
};

static auto UserStateToString(UserState state) -> std::string {
    switch (state) {
        case UserState::OFFLINE: return "offline";
        case UserState::ONLINE: return "online";
        default: return "INVALID";
    }
}



static auto StringToUserState(std::string state) -> UserState {
    if (state == "offline") return UserState::OFFLINE;
    else if (state == "online") return UserState::ONLINE;
    return UserState::INVALID;
}


enum GroupUserRole {
    CREATOR = 0,
    NORMAL = 1,
    INVALID_GroupUserRole = -1,
};

static auto GroupUserRoleToString(GroupUserRole state) -> std::string {
    switch (state) {
        case GroupUserRole::CREATOR: return "creator";
        case GroupUserRole::NORMAL: return "normal";
        default: return "INVALID_GroupUserRole";
    }
}

static auto StringToGroupUser(std::string state) -> GroupUserRole {
    if (state == "creator") return GroupUserRole::CREATOR;
    else if (state == "normal") return GroupUserRole::NORMAL;
    return INVALID_GroupUserRole;
}