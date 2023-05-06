#pragma once
#include <iostream>

#include "public.hpp"

class User {
public:
    User(int id = -1, std::string name = "", std::string pwd = "", UserState state = UserState::OFFLINE)
    : id_(id), name_(name), pwd_(pwd), state_(state)
    {}

    void setId(int id) { this->id_ = id; }
    void setName(const std::string& name) { this->name_ = name; }
    void setPwd(const std::string& pwd) { this->pwd_ = pwd; }
    void setState(UserState state) { this->state_ = state; }

    auto getId() -> int const  { return id_; }
    auto getName() -> std::string const  { return name_; }
    auto getPwd() -> std::string const  { return pwd_; }
    auto getState() -> UserState const  { return state_; }

private:
    int id_{-1};
    std::string name_{""};
    std::string pwd_{""};
    UserState state_{UserState::OFFLINE};
};