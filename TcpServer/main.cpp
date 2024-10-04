#include "tcpserver.h"
#include <QApplication>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpServer w;
    w.setWindowTitle("服务器");
    w.show();
    return a.exec();
}
