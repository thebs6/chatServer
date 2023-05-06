#include "friendmodel.hpp"
#include "db.hpp"

void FriendModel::insert(int userId, int friendId){
    char sql[1024] = {0};
    sprintf(sql, "insert into friend values(%d, %d)", userId, friendId);

    MySQL mysql;
    if (mysql.connect()) {
        mysql.update(sql);
    }
}

auto FriendModel::query(int userId) -> std::vector<User> {
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.name, a.state from user a inner join friend b on b.friendid = a.id where b.userid=%d", userId);

    vector<User> vec;
    MySQL mysql;
    if (mysql.connect()) {
        MYSQL_RES* res = mysql.query(sql);
        if(res != nullptr) {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr) {
                User user;
                user.setId(atoi(row[0]));
                user.setName(row[1]);
                user.setState(StringToUserState(row[2]));
                vec.push_back(user);
            }
            mysql_free_result(res);
            return vec;
        }
    }
    return vec;
}