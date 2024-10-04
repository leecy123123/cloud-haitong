#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include"QTcpServer.h"
#include"QDebug"
#include"QList"
#include"mytcpsocket.h"
#include"protocol.h"


// 自定义TcpServer类
class MyTcpServer : public QTcpServer
{
    // 添加了该宏定义才可以使用槽函数
    Q_OBJECT

public:
    MyTcpServer();

    // 获取自定义TCPServer单例实现类
    static MyTcpServer &getInstance();

    // 新服务端链接时触发方法
    void incomingConnection(qintptr socketDescriptor);

    // 转发消息
    void resend(const char *friendName, PDU *pdu);

public slots:
    // 删除链接的客户端
    void deleteSocket(MyTcpSocket *myTcpSocket);

private:
    QList<MyTcpSocket*> m_tcpSocketList;//存储接受到的socket对象
};

#endif // MYTCPSERVER_H
