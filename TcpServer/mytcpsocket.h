#ifndef MYTCPSOCKET_H
#define MYTCPSOCKET_H
#include"protocol.h"
#include"QTcpSocket"
#include"opedb.h"
#include"QString"
#include<QStringList>
#include<QFile>
#include<QTimer>

// 自定义TCPSocket类
class MyTcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    MyTcpSocket();
    // 获取用户姓名
    QString getStrName();
// 自定义信号
signals:
    offline(MyTcpSocket *mySocket);

public slots:

    // 接受到数据后的操作
    void recvMsg();

    // 客户端退出链接
    void clientOffline();

    // 发送文件数据给客户端
    void sendFileDataToClient();

private:
    QString m_strName;//用户名
    // 组测
    void regist(PDU *pdu);
    void login(PDU *pdu);
    void allOnline(PDU *pdu);
    void searchUser(PDU *pdu);
    void addFriend(PDU *pdu);
    void addFriendAgree(PDU *pdu);
    void addFriendRefuse(PDU *pdu);
    void flushFriend(PDU *pdu);
    void deleteFriend(PDU *pdu);
    void privateChat(PDU *pdu);
    void groupChat(PDU *pdu);
    void createDir(PDU *pdu);
    void flushDir(PDU *pdu);
    void delDir(PDU *pdu);
    void renameDir(PDU *pdu);
    void enterDir(PDU *pdu);
    // 将指定路径文件夹中的所有文件写入PDU中，并返回这个PDU
    PDU *getDirFilePDU(QString curPath);
    void uploadFile(PDU *pdu);
    void delFile(PDU *pdu);
    void downloadFile(PDU *pdu);
    void shareFile(PDU *pdu);
    void shareFileNote(PDU *pdu);
    void copyDir(QString sourceDir, QString targetDir);

    QFile m_file;//上传的文件
    qint64 m_iTotal;//文件总大小
    qint64 m_iRecved;//已接受到的数据大小
    bool m_bUpload; //正在上传文件的状态
    qint64 m_iCount;//计数
    QTimer *m_pTimer;
};

#endif // MYTCPSOCKET_H
