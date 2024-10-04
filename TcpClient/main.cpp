#include "tcpclient.h"
#include <QApplication>
#include"sharefile.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpClient &w = TcpClient::getInstance();
//    ShareFile w;
//    QListWidget l;
//    l.addItem("你哦好");
//    l.addItem("你哦好1");
//    l.addItem("你哦好2");
//    w.updateFriend(&l);
    w.setWindowTitle("客户端");
    w.show();
    return a.exec();
}
