#pragma once

#include "groupuser.hpp"

#include <string>
#include <vector>
class Group {
public:
    Group(){}
    Group(int id, std::string group_name, std::string group_desc) : 
    id_(id), group_name_(group_name), group_desc_(group_desc)
    {}

    auto getId() const -> int {
        return id_;
    }

    auto getGroupName() const -> std::string {
        return group_name_;
    }

    auto getGroupDesc() const -> std::string {
        return group_desc_;
    }

    void setId(int id) {
        id_ = id;
    }

    void setGroupName(std::string group_name) {
        group_name_ = group_name;
    }

    void setGroupDesc(std::string group_desc)  {
        group_desc_ = group_desc;
    }

    auto getGroupUsers() -> std::vector<GroupUser>& { return users_; }

private:
    int id_{-1};
    std::string group_name_{};
    std::string group_desc_{};

    std::vector<GroupUser> users_;
};