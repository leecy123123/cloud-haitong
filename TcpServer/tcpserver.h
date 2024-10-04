#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QWidget>
#include"QTcpServer"
#include"mytcpserver.h"
#include "QFile"
#include"QByteArray"
#include"QDebug"
#include"QString"
#include"QStringList"
#include"QHostAddress"
#include"QMessageBox"
#include"QTcpSocket"

namespace Ui {
class TcpServer;
}

class TcpServer : public QWidget
{
    Q_OBJECT

public:
    // 指定只能有这一个构造函数，不能有其他构造函数
    explicit TcpServer(QWidget *parent = 0);
    // 析构函数
    ~TcpServer();
    // 加载配置文件
    void loadConfig();

private:
    Ui::TcpServer *ui;
    QString m_strIp;//服务端的ip
    quint16 m_usPort;//服务端的port
};

#endif // TCPSERVER_H
