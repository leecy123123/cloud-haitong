#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QWidget>
#include"QTcpSocket"
#include "QFile"
#include"QByteArray"
#include"QDebug"
#include"QString"
#include"QStringList"
#include"QHostAddress"
#include"QMessageBox"
#include"protocol.h"
#include"opewidget.h"

namespace Ui {
class TcpClient;
}

class TcpClient : public QWidget
{
    Q_OBJECT

public:
    // 指定只能有这一个构造函数，不能有其他构造函数
    explicit TcpClient(QWidget *parent = 0);
    // 析构函数
    ~TcpClient();
    // 加载配置文件
    void loadConfig();
    // 单例获取
    static TcpClient &getInstance();
    // 获取当前客户端的TcpSocket
    QTcpSocket &getTcpSocket();
    // 获取登录用户名称
    QString loginName();
    // 获取当前路径
    QString curPath();
    // 设置当前路径
    void setCurPath(QString strPath);

// 槽函数
public slots:

    // 链接成功提示
    void showConnect();

    // 接受到消息的处理
    void recvMsg();

private slots:
    void on_login_pb_clicked();

    void on_regist_pb_clicked();

    void on_cancel_pb_clicked();

    void on_pwd_le_returnPressed();

private:
    Ui::TcpClient *ui;
    QTcpSocket m_tcpSocket;//链接服务器
    QString m_strIp;//服务端的ip
    quint16 m_usPort;//服务端的port
    QString m_strLoginName;//登录用户名
    QString m_strCurPath;//当前文件路径
    // 注册
    void regist(PDU *pdu);
    // 注册
    void login(PDU *pdu);
    // 查找用户
    void searchUser(PDU *pdu);
    // 添加好友：获得转发
    void addFriendRequest(PDU *pdu);
    // 同意添加好友
    void addFriendAgree(PDU *pdu);
    // 拒绝添加好友
    void addFriendFefuse(PDU *pdu);
    // 删除好友
    void deleteFriend(PDU *pdu);
    // 私聊
    void privateChat(PDU *pdu);
    // 创建文件夹
    void createDir(PDU *pdu);
    // 刷新文件夹
    void flushDir(PDU *pdu);
    // 删除文件夹
    void delDir(PDU *pdu);
    // 重命名文件
    void renameDir(PDU *pdu);
    // 进入文件夹
    void enterDir(PDU *pdu);
    // 下载文件的预处理事件
    void downloadFilePre(PDU *pdu);
    // 好友接受分享文件的提示
    void shareFileNote(PDU *pdu);
};

#endif // TCPCLIENT_H
