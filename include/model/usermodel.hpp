#pragma once

#include "user.hpp"
class UserModel {
public:
    auto insert(User &user) -> bool;
    auto query(int id) -> User;
    auto updateState(User &user) -> bool;
    auto resetState() -> bool;
};