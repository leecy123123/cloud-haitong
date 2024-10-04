#ifndef OPEDB_H
#define OPEDB_H

#include"QObject"
#include"QSqlDatabase"
#include"QSqlQuery"
#include<QStringList>

// 自定义数据库操作类
class OpeDB : public QObject
{
    Q_OBJECT
public:
    explicit OpeDB(QObject *parent = 0);
    ~OpeDB();

    // 单例获取
    static OpeDB& getInstance();

    // 初始化数据库链接
    void init();

    // 注册账号
    bool handleRegist(const char *name, const char *pwd);
    // 登录操作
    bool handleLogin(const char *name, const char *pwd);
    // 用户注销
    void handleOffine(const char *name);
    // 获取所有在线用户
    QStringList handleAllOnline();
    // 返回查找用户结果
    int handleSearchUser(const char *name);
    // 添加好友--校验
    int handleAddfriendCheck(const char *friendName, const char *loginName);
    // 添加好友操作
    void handleAddfriend(const char *friendName, const char *loginName);
    // 刷新好友列表
    QStringList handleFlushFriend(const char *name);
    // 删除好友操作
    void handleDeletefriend(const char *friendName, const char *loginName);
signals:

public slots:

private:
    QSqlDatabase m_db;//连接数据库
};

#endif // OPEDB_H
