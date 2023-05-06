#include "groupmodel.hpp"
#include "db.hpp"
#include "public.hpp"
#include <mysql/mysql.h>

bool GroupModel::createGroup(Group& group) {
    char sql[1024] = {0};
    sprintf(sql, "insert into allgroup(groupname, groupdesc) values('%s', '%s')", 
            group.getGroupName().c_str(), group.getGroupDesc().c_str());

    MySQL mysql;
    if(mysql.connect()) {
        if(mysql.update(sql)) {
            group.setId(mysql_insert_id(mysql.getConnection()));
            return true;
        }
    }
    return false;
}

void GroupModel::addGroup(int userId, int groupId, std::string role) {
    char sql[1024] = {0};
    sprintf(sql, "insert into groupuser(groupid, userid, grouprole) values('%d', '%d', '%s')", 
            userId, groupId, role.c_str());

    MySQL mysql;
    if(mysql.connect()) {
        mysql.update(sql);
    }
}

std::vector<Group> GroupModel::queryGroups(int userId) {
    char sql[1024] = {0};
    sprintf(sql, "select a.id, a.groupname, a.groupdesc from allgroup a inner join groupuser b on a.id = b.groupid where b.userid=%d", 
            userId);
    MySQL mysql;
    vector<Group> groupVec;
    if(mysql.connect()) {
        auto res = mysql.query(sql);
        if(res != nullptr) {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr) {
                Group group;
                group.setId(atoi(row[0]));
                group.setGroupName(row[1]);
                group.setGroupDesc(row[2]);
                groupVec.push_back(group);
            }
            mysql_free_result(res);
        }
    }

    for(auto& group : groupVec) {
        sprintf(sql, "select a.id, a.name, a.state, b.grouprole from user a inner join groupuser b on b.userid = a.id where b.groupid=%d", group.getId());

        auto res = mysql.query(sql);

        if(res != nullptr) {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr) {
                GroupUser group_user;
                group_user.setId(atoi(row[0]));
                group_user.setName(row[1]);
                group_user.setState(StringToUserState(row[2]));
                group_user.setRole(StringTOGroupUser(row[3]));
                group.getGroupUsers().push_back(group_user);
            }
            mysql_free_result(res);
        }
    }

    return groupVec;
}

std::vector<int> GroupModel::queryGroupUsers(int userId, int groupId) {
    char sql[1024] = {0};
    sprintf(sql, "select userid from groupuser where groupid = %d and userid != %d", groupId, userId);

    vector<int> idVec;
    MySQL mysql;
    if (mysql.connect())
    {
        MYSQL_RES *res = mysql.query(sql);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                idVec.push_back(atoi(row[0]));
            }
            mysql_free_result(res);
        }
    }
    return idVec;
}