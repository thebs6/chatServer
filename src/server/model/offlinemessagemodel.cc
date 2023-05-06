#include "offlinemessagemodel.hpp"
#include "db.hpp"
#include <mysql/mysql.h>
#include <iostream>

void OfflineMsgModel::insert(int uid, std::string msg) {
    char sql[1024] = {0};
    sprintf(sql, "insert into offlinemessage(userid, message) values('%d', '%s')",
            uid, msg.c_str());
    MySQL mysql;
    if(mysql.connect()) {
        mysql.update(sql);
    }
}

void OfflineMsgModel::remove(int uid) {
    char sql[1024] = {0};
    sprintf(sql, "delete from offlinemessage where userid=%d", uid);
    MySQL mysql;
    if(mysql.connect()) {
        mysql.update(sql);
    }
}

auto OfflineMsgModel::query(int uid) -> std::vector<std::string> {
    char sql[1024] = {0};
    sprintf(sql, "select * from offlinemessage where userid=%d", uid);
    MySQL mysql;
    std::vector<std::string> msgs;
    if(mysql.connect()) {
        auto res = mysql.query(sql);
        if(res != nullptr) {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr) {
                msgs.emplace_back(row[1]);
            }
            mysql_free_result(res);
            return msgs;
        }
    }
    return msgs;
}