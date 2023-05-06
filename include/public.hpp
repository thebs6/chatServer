#pragma once

#include <string>
enum EnMsgType {
    LOGIN_MSG = 1,
    REG_MSG,
    REG_MSG_ACK,
    LOGIN_ACK,
    LOGIN_MSG_ACK,
    ONE_CHAT_MSG,
    ADD_FRIEND_MSG,
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
    CREATER = 0,
    NORMAL = 1,
    INVALID_GroupUserRole = -1,
};

static auto GroupUserRoleToString(GroupUserRole state) -> std::string {
    switch (state) {
        case GroupUserRole::CREATER: return "creater";
        case GroupUserRole::NORMAL: return "normal";
        default: return "INVALID_GroupUserRole";
    }
}

static auto StringTOGroupUser(std::string state) -> GroupUserRole {
    if (state == "creater") return GroupUserRole::CREATER;
    else if (state == "normal") return GroupUserRole::NORMAL;
    return INVALID_GroupUserRole;
}