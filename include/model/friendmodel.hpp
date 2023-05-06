#pragma once

#include "user.hpp"
#include <vector>

class FriendModel {
public:
    void insert(int userId, int friendId);
    auto query(int userId) -> std::vector<User>;
};