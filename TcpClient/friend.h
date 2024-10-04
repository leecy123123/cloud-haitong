#ifndef FRIEND_H
#define FRIEND_H

#include <QWidget>
#include<QTextEdit>
#include<QListWidget>
#include<QLineEdit>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include"online.h"
#include"protocol.h"

class Friend : public QWidget
{
    Q_OBJECT
public:
    explicit Friend(QWidget *parent = 0);
    // 显示所有在线用户
    void showAllOnlineUser(PDU *pdu);
    QString m_strSearchName;//输入搜索用户的名称
    // 刷新好友列表
    void updateFriendList(PDU *pdu);
    // 刷新群聊消息
    void updateGroupMsg(PDU *pdu);
    QListWidget *getFriendList();
signals:

public slots:
    // 切换在线用户显示/隐藏状态
    void showOnline();
    // 查询用户
    void searchUser();
    // 刷新好友
    void flushFriend();
    // 删除好友
    void deleteFriend();
    // 私聊
    void privateChat();
    // 群聊
    void groupChat();
private:
    QTextEdit *m_pShowMsgTE;//显示信息
    QListWidget *m_pFriendListWidget;//好友列表
    QLineEdit *m_pInputMsgLE;//内容输入框
    QPushButton *m_pDelFriendPB;//删除用户按钮
    QPushButton *m_pFlushFriendPB;//刷新好友用户按钮
    QPushButton *m_pShowOnlineUserPB;//查看在线用户
    QPushButton *m_pSearchUserPB;//搜索用户
    QPushButton *m_pMsgSendPB;//信息发送(群聊）
    QPushButton *m_pPrivateChatPB;//私聊

    Online *m_pOnline;//在线用户

};

#endif // FRIEND_H
