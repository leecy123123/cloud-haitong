#include "mytcpserver.h"

MyTcpServer::MyTcpServer()
{

}

MyTcpServer &MyTcpServer::getInstance()
{
    static MyTcpServer instance;
    return instance;
}

void MyTcpServer::incomingConnection(qintptr socketDescriptor)
{
    // 测试输出
    qDebug() << "new client connect";

    // 创建自定义的TCPSocket对象
    MyTcpSocket *pTcpSocket = new MyTcpSocket;

    // 将接受到的TCPSocket对象保存到自定义的TCPSocket对象中
    pTcpSocket->setSocketDescriptor(socketDescriptor);

    // 将创建的TCPSocket对象添加到存储的List中
    m_tcpSocketList.append(pTcpSocket);

    // 发出端口链接信号
    connect(pTcpSocket, SIGNAL(offline(MyTcpSocket*)), this, SLOT(deleteSocket(MyTcpSocket*)));
}

void MyTcpServer::resend(const char *friendName, PDU *pdu)
{
    if(NULL == friendName || NULL == pdu)
    {
        return ;
    }
    QString qStrFriendName = friendName;

    // 在m_tcpSocketList里找到要转发给的人
    for(int i = 0; i < m_tcpSocketList.size(); i++)
    {
        if(qStrFriendName == m_tcpSocketList.at(i)->getStrName())
        {
            m_tcpSocketList.at(i)->write((char*)pdu, pdu->uiPDULen);
            break;
        }
    }
}

void MyTcpServer::deleteSocket(MyTcpSocket *myTcpSocket)
{
    // 获取链表迭代器
    QList<MyTcpSocket*>::iterator iter = m_tcpSocketList.begin();
    for(; iter != m_tcpSocketList.end(); iter++)
    {
        // 如果找到了这个客户端对象，则删除这个对象
        if(myTcpSocket == *iter)
        {
            // 如果客户端还在和服务器通信的时候直接断掉，会导致程序崩溃，故使用 deleteLater 函数可以
            // 在最后一次通信结束后断开链接，然后释放这片空间
            (*iter)->deleteLater();
//            delete *iter;
//            *iter = NULL;
            m_tcpSocketList.erase(iter);
            break;
        }
    }
}
