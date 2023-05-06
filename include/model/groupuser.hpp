#pragma once

#include "public.hpp"
#include "user.hpp"

class GroupUser : public User {
public:
    void setRole(GroupUserRole role) { role_ = role; }
    auto getRole() const -> GroupUserRole { return role_; }
private:
    GroupUserRole role_{};
};