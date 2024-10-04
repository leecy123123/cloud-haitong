#include "tcpserver.h"
#include "ui_tcpserver.h"

TcpServer::TcpServer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpServer)
{
    ui->setupUi(this);
    // 加载配置文件
    loadConfig();


    // 服务端开启监听端口链接
    MyTcpServer::getInstance().listen(QHostAddress(m_strIp), m_usPort);
}

TcpServer::~TcpServer()
{
    delete ui;
}

void TcpServer::loadConfig()
{
    // : 代表读取的是资源文件
    // 设置文件路径
    QFile file(":/server.config");
    // 设置打开方式，尝试打开
    if(file.open(QIODevice::ReadOnly)) {
        // 进行文件读取
        QByteArray btData = file.readAll();
        // 转换为 char *
        QString fileStrData = btData.toStdString().c_str();
        // 将换行替换为空格
        fileStrData.replace("\r\n", " ");
        // 有些人的环境换行是 \n,所以再加一步替换，防止切分失败
        fileStrData.replace("\n", " ");
        // 按行切分数据
        QStringList strList = fileStrData.split(" ");
        // 获取ip
        m_strIp = strList.at(0);
        // 获取port
        m_usPort = strList.at(1).toUShort();
        // 测试输出
        qDebug() << fileStrData << "\n";
    // 如果打开失败
    } else {
        // 错误提示框，和information相比就是界面的图片变成了红色的感叹号
        QMessageBox::critical(this, "错误", "加载配置文件错误");
    }
    // 关闭文件
    file.close();
}
