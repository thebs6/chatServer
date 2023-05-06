#pragma once

#include <string>
#include <vector>
class OfflineMsgModel {
public:
    void insert(int uid, std::string msg);

    void remove(int uid);

    std::vector<std::string> query(int uid);
};