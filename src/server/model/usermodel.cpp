#include "usermodel.hpp"
#include "public.hpp"
#include "db.hpp"
#include <mysql/mysql.h>

bool UserModel::insert(User &user) {
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, password, state) values('%s', '%s', '%s')",
            user.getName().c_str(), user.getPwd().c_str(), UserStateToString(user.getState()).c_str());
    MySQL mysql;
    if(mysql.connect()) {
        if(mysql.update(sql)) {
            user.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }

    return false;
}

User UserModel::query(int id) {
    char sql[1024] = {0};
    sprintf(sql, "select * from user where id = %d", id);

    MySQL mysql;
    if(mysql.connect()) {
        MYSQL_RES *res = mysql.query(sql);
        if(res != nullptr) {
            auto row = mysql_fetch_row(res);
            if (row != nullptr) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setPwd(row[2]);
                user.setState(StringToUserState(row[3]));
                return user;
            }
        }
    }
    return User();
}

auto UserModel::updateState(User &user) -> bool {
    char sql[1024] = {0};
    sprintf(sql, "update user set state = '%s' where id = %d", UserStateToString(user.getState()).c_str(), user.getId());
    std::cout << sql << std::endl;
    MySQL mysql;
    if(mysql.connect()) {
        if(mysql.update(sql)) {
            return true;
        }
    }
    return false;
}

auto UserModel::resetState() -> bool {
    char sql[1024] = "update user set state = 'offline' where state = 'online'";
    MySQL mysql;
    if(mysql.connect()) {
        if(mysql.update(sql)) {
            return true;
        }
    }
    return false;
}