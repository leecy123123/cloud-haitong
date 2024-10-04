#include "opedb.h"
#include"QMessageBox"
#include"QDebug"
#include"QString"

OpeDB::OpeDB(QObject *parent) : QObject(parent)
{
    // 添加数据库，指定要操作的数据库是sqlite3
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    // 初始化数据库
    init();
}

OpeDB::~OpeDB()
{
    // 关闭数据库
    m_db.close();
}

OpeDB &OpeDB::getInstance()
{
    static OpeDB instance;
    return instance;
}

void OpeDB::init()
{
    m_db.setHostName("localhost");
    // :代表读取资源文件下的内容
    m_db.setDatabaseName("F:/wangpan/cloud-storage-haitong/TcpServer/cloud.db");
    if(m_db.open())
    {
        QSqlQuery query;
        query.exec("select * from userInfo");
        while(query.next())
        {
            QString data = QString("%1,%2,%3").arg(query.value(0).toString())
                    .arg(query.value(1).toString())
                    .arg(query.value(2).toString());
            qDebug() << data;
        }
    }
    else
    {
        QMessageBox::critical(NULL, "打开数据库", "打开数据库失败");
    }
}

bool OpeDB::handleRegist(const char *name, const char *pwd)
{
    // 非空校验
    if(NULL == name || NULL == pwd)
    {
        return false;
    }
    QString strSql = QString("insert into userInfo(name, pwd) values('%1','%2')").arg(name).arg(pwd);
    QSqlQuery query;
    // 返回sql插入结果
    return query.exec(strSql);
}

bool OpeDB::handleLogin(const char *name, const char *pwd)
{
    // 非空校验
    if(NULL == name || NULL == pwd)
    {
        return false;
    }
    QString strSql = QString("select * from userInfo where name = '%1' and pwd = '%2' and online = 0").arg(name).arg(pwd);
    QSqlQuery query;
    query.exec(strSql);
    // 判断是否查询到了数据,next()就是能一条一条查询sql里的数据，查到了就为真，到最后都没有查到就为假
    if(query.next())
    {
        // 如果查询到了数据，则更新用户的登录状态
        strSql = QString("update userInfo set online = 1 where name = '%1'").arg(name);
        query.exec(strSql);
        return true;
    }
    else
    {
        return false;
    }
}

void OpeDB::handleOffine(const char *name)
{
    // 非空校验
    if(NULL == name)
    {
        return;
    }
    QString strSql = QString("update userInfo set online = 0 where name = '%1'").arg(name);
    QSqlQuery query;
    // 执行更新操作
    query.exec(strSql);
}

QStringList OpeDB::handleAllOnline()
{
    QString strSql = QString("select name from userInfo where online = 1");

    QSqlQuery query;

    // 执行操作
    query.exec(strSql);

    QStringList result;
    result.clear();

    // 把在线用户的名字转化成字符串都放进result中
    while(query.next())
    {
        result.append(query.value(0).toString());
    }
    return result;
}

int OpeDB::handleSearchUser(const char *name)
{
    // 用户不存在
    if(NULL == name)
    {
        return -1;
    }
    QString strSql = QString("select online from userInfo where name = '%1'").arg(name);
    QSqlQuery query;
    // 执行操作
    query.exec(strSql);
    // 如果查询到结果了的话
    if(query.next())
    {
        // 返回用户在线结果
        return query.value(0).toInt();
    }
    // 用户不存在
    else
    {
        return -1;
    }
}

int OpeDB::handleAddfriendCheck(const char *friendName, const char *loginName)
{
    // 输入内容格式错误
    if(NULL == friendName || NULL == loginName)
    {
        return -1;
    }
    QString strSql = QString("select id from userInfo where name = '%1'").arg(friendName);
    QSqlQuery query;
    query.exec(strSql);
    if(query.next())
    {
        int friendId = query.value(0).toInt();
        strSql = QString("select id from userInfo where name = '%1'").arg(loginName);
        query.exec(strSql);
        query.next();
        int loginId = query.value(0).toInt();
        strSql = QString("select * from friend where id = '%1' and friendId = '%2'").arg(loginId).arg(friendId);
        query.exec(strSql);
        if(query.next())
        {
            // 如果已经是好友了
            return 0;
        }
        else
        {
            strSql = QString("select online from userInfo where id = '%1'").arg(friendId);
            query.exec(strSql);
            query.next();
            int online = query.value(0).toInt();
            if(online == 1)
            {
                // 对方在线中
                return 1;
            }
            else if(online == 0)
            {
                // 如果对方离线中
                return 2;
            }
        }
    }
    else
    {
        // 用户不存在
        return 3;
    }
}

void OpeDB::handleAddfriend(const char *friendName, const char *loginName)
{
    // 获取想要加的好友的id
    QString strSql = QString("select id from userInfo where name = '%1'").arg(friendName);
    QSqlQuery query;
    query.exec(strSql);
    query.next();
    int friendId = query.value(0).toInt();
    // 获取登录用户id
    strSql = QString("select id from userInfo where name = '%1'").arg(loginName);
    query.exec(strSql);
    query.next();
    int loginId = query.value(0).toInt();
    // 新增好友
    strSql = QString("insert into friend (id,friendId) values ('%1','%2'), ('%2','%1')").arg(friendId).arg(loginId);
    query.exec(strSql);
}


QStringList OpeDB::handleFlushFriend(const char *name)
{
    QStringList strFriendNameList;
    strFriendNameList.clear();

    if(NULL == name)
    {
        return strFriendNameList;
    }

    QString strSql = QString("select id from userInfo where name = '%1'").arg(name);
    QSqlQuery query;
    query.exec(strSql);
    query.next();
    int loginId = query.value(0).toInt();

    strSql = QString("select friendId from friend where id = '%1'").arg(loginId);
    query.exec(strSql);
    while(query.next())
    {
        int friendId = query.value(0).toInt();
        strSql = QString("select name from userInfo where id = '%1' and online = 1").arg(friendId);
        QSqlQuery nameQuery;
        nameQuery.exec(strSql);
        if(nameQuery.next())
        {
            strFriendNameList.append(nameQuery.value(0).toString());
        }
    }
    return strFriendNameList;
}


void OpeDB::handleDeletefriend(const char *friendName, const char *loginName)
{
    if(NULL == friendName || NULL == loginName)
    {
        return ;
    }

    // 通过好友的名字查到好友的id，放到friendId里
    QString strSql = QString("select id from userInfo where name = '%1'").arg(friendName);
    QSqlQuery query;
    query.exec(strSql);
    query.next();
    int friendId = query.value(0).toInt();

    // 通过自己的名字查到自己的id，放到userId里
    strSql = QString("select id from userInfo where name = '%1'").arg(loginName);
    query.exec(strSql);
    query.next();
    int userId = query.value(0).toInt();

    // 在friend表里删除这个关系
    strSql = QString("delete from friend where (id = '%1' and friendId = '%2') or (id = '%2' and friendId = '%1')").arg(userId).arg(friendId);
    query.exec(strSql);
}
