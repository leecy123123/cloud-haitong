#include "tcpclient.h"
#include "ui_tcpclient.h"
#include"privatechat.h"
#include<QFile>

TcpClient::TcpClient(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TcpClient)
{
    ui->setupUi(this);

    // 大小
    resize(500, 300);

    // 加载配置文件
    loadConfig();

    // 定义服务器链接成功槽函数
    connect(&m_tcpSocket, SIGNAL(connected()), this, SLOT(showConnect()));

    // 从服务器端接受到数据，对应的槽函数
    connect(&m_tcpSocket, SIGNAL(readyRead()), this, SLOT(recvMsg()));

    // 链接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIp), m_usPort);
}

TcpClient::~TcpClient()
{
    delete ui;
}

void TcpClient::loadConfig()
{
    // : 代表读取的是资源文件
    // 设置文件路径
    QFile file(":/client.config");
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


// 写成一个单例模式，方便其他地方调用
TcpClient &TcpClient::getInstance()
{
    static TcpClient instance;
    return instance;
}

// 返回m_tcpSocket的引用，哪里需要用m_tcpSocket，只需要调用该函数，把引用的对象m_tcpSocket返回
// 就可以使用m_tcpSocket收发数据
QTcpSocket &TcpClient::getTcpSocket()
{
    return m_tcpSocket;
}

QString TcpClient::loginName()
{
    return m_strLoginName;
}

QString TcpClient::curPath()
{
    return m_strCurPath;
}

void TcpClient::setCurPath(QString strPath)
{
    m_strCurPath= strPath;
}

void TcpClient::showConnect()
{
    QMessageBox::information(this, "成功", "TCP链接服务端成功");
}

void TcpClient::regist(PDU *pdu) {
    if(0 == strcmp(pdu->caData, REGIST_OK))
    {
        QMessageBox::information(this, "注册", REGIST_OK);
    }
    else if(0 == strcmp(pdu->caData, REGIST_FAIL))
    {
        QMessageBox::warning(this, "注册", REGIST_FAIL);
    }
}

void TcpClient::login(PDU *pdu)
{
    if(0 == strcmp(pdu->caData, LOGIN_OK))
    {
        m_strCurPath = QString("./%1").arg(m_strLoginName);
        QMessageBox::information(this, "登录", LOGIN_OK);

        // 打开操作界面
        OpeWidget::getInstance().show();

        // 登录成功之后，把tcpclient对象隐藏掉
        this->hide();
    }
    else if(0 == strcmp(pdu->caData, LOGIN_FAIL))
    {
        QMessageBox::warning(this, "登录", LOGIN_FAIL);
    }
}

void TcpClient::searchUser(PDU *pdu)
{
    if(0 == strcmp(SEARCH_USER_NO, pdu->caData))
    {
        QMessageBox::information(this, "搜索", QString("%1: no exists").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
    }
    else if(0 == strcmp(SEARCH_USER_ONLINE, pdu->caData))
    {
        QMessageBox::information(this, "搜索", QString("%1: online").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
    }
    else if(0 == strcmp(SEARCH_USER_OFFLINE, pdu->caData))
    {
        QMessageBox::information(this, "搜索", QString("%1: offline").arg(OpeWidget::getInstance().getFriend()->m_strSearchName));
    }
}

void TcpClient::addFriendRequest(PDU *pdu)
{
    char caName[32] = {'\0'};//请求添加好友的名称

    // 把clientA端发给server端的pdu中的caData中的后32位（clientA自己的名字），存放到caName里
    strncpy(caName, pdu->caData + 32, 32);

    int ret = QMessageBox::information(this,
                                       "添加好友",
                                       QString("%1 want to add you as friend.").arg(caName),
                                       QMessageBox::Yes,
                                       QMessageBox::No);
    PDU *retPdu = mkPDU(0);
    memcpy(retPdu->caData, pdu->caData, 64);
    if(ret == QMessageBox::Yes)
    {
        retPdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_AGREE;
    }
    else
    {
        retPdu->uiMsgType = ENUM_MSG_TYPE_ADD_FRIEND_REFUSE;
    }
    m_tcpSocket.write((char*)retPdu, retPdu->uiPDULen);

    free(retPdu);
    retPdu = NULL;
}

void TcpClient::addFriendAgree(PDU *pdu)
{
    char friendName[32] = {'\0'};//好友的名称
    strncpy(friendName, pdu->caData, 32);
    QMessageBox::information(this, "添加好友", QString("添加好友成功！%1 同意了您的好友添加请求").arg(friendName));
}

void TcpClient::addFriendFefuse(PDU *pdu)
{
    char friendName[32] = {'\0'};//好友的名称
    strncpy(friendName, pdu->caData, 32);
    QMessageBox::information(this, "添加好友", QString("添加好友失败！%1 拒绝了您的好友添加请求").arg(friendName));
}

void TcpClient::deleteFriend(PDU *pdu)
{
    char loginName[32] = {'\0'};//登录者的名称
    strncpy(loginName, pdu->caData + 32, 32);
    QMessageBox::information(this, "删除好友", QString("%1 已经删除了您的好友").arg(loginName));
}

void TcpClient::privateChat(PDU *pdu)
{
    if(PrivateChat::getInstace().isHidden())
    {
        PrivateChat::getInstace().show();
    }

    // 给最新的一个发送过来的好友发送消息
    char loginName[32] = {'\0'};
    // A发送给B，此处loginName为A
    memcpy(loginName, pdu->caData + 32, 32);
    PrivateChat::getInstace().setChatName(loginName);
    PrivateChat::getInstace().updateMsg(pdu);
}

void TcpClient::createDir(PDU *pdu)
{
    QMessageBox::information(this, "创建文件夹", pdu->caData);
}

void TcpClient::flushDir(PDU *pdu)
{
    OpeWidget::getInstance().getBook()->updateDirList(pdu);
}

void TcpClient::delDir(PDU *pdu)
{
    QMessageBox::information(this, "删除文件夹", pdu->caData);
}

void TcpClient::renameDir(PDU *pdu)
{
    QMessageBox::information(this, "重命名文件", pdu->caData);
}

void TcpClient::enterDir(PDU *pdu)
{
    // 如果请求成功了
    if(0 == strcmp(ENTER_DIR_OK,pdu->caData))
    {
        setCurPath(OpeWidget::getInstance().getBook()->getEnterPath());
        OpeWidget::getInstance().getBook()->updateDirList(pdu);
    }
    else
    {
        QMessageBox::information(this, "进入文件夹", pdu->caData);
    }
}

void TcpClient::downloadFilePre(PDU *pdu)
{
    char caFileName[32] = {'\0'};
    // 把要下载的文件名和大小提取出来
    sscanf(pdu->caData, "%s %lld", caFileName, &(OpeWidget::getInstance().getBook()->m_iTotal));
    // 如果要下载的文件是有效数据
    if(strlen(caFileName) > 0 && OpeWidget::getInstance().getBook()->m_iTotal > 0)
    {
        OpeWidget::getInstance().getBook()->setDownloadStatus(true);//标记开始下载文件
        OpeWidget::getInstance().getBook()->m_iRevice = 0;
        OpeWidget::getInstance().getBook()->updateLocalDownloadFileName();
        // 只写模式打开文件，文件如果不存在则会被创建
        if(!OpeWidget::getInstance().getBook()->m_pFile.open(QIODevice::WriteOnly))
        {
            QMessageBox::warning(this, "下载文件", "下载文件失败：本地文件无法操作");
            // 由于服务器已经开始发送下载文件的数据了，这个时候应该怎么办呢？【】
            return ;
        }
    }
}

void TcpClient::shareFileNote(PDU *pdu)
{
    qDebug() << "开始准备接受文件";

    // 将路径从server端发送过来的retPDU中提取出来
    char *pPath = new char[pdu->uiMsgLen];
    memcpy(pPath, (char*)pdu->caMsg, pdu->uiMsgLen);

    // aa/bb/cc/a.txt
    qDebug() << pPath;
    char *pos = strrchr(pPath, '/'); //找到最后一个 / 出现的位置
    qDebug() << pos;
    if(NULL != pos)
    {
        // 表示找到了
        pos++; // 向右移动一位，因为 / 这个字符我们不需要，只需要文件名称，即a.txt
        QString strNote = QString("%1 share file -> %2\n Do you accecpt?").arg(pdu->caData).arg(pos);

        // ret接受前端用户的选择，是yes还是no
        // 如果选择yes，就给server端一个回复
        int ret = QMessageBox::question(this, "共享文件", strNote);
        if(QMessageBox::Yes == ret)
        {
            PDU *retPdu = mkPDU(pdu->uiMsgLen);
            retPdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_NOTE_REQUEST;

            // 接受的文件路径在哪里
            memcpy(retPdu->caMsg, pdu->caMsg, pdu->uiMsgLen);

            // 自己是谁
            QString strName = TcpClient::getInstance().loginName();
            strcpy(retPdu->caData, strName.toStdString().c_str());

            m_tcpSocket.write((char*)retPdu,retPdu->uiPDULen);
        }
    }
}

void TcpClient::recvMsg()
{
    // 如果是文件下载状态中
    if(OpeWidget::getInstance().getBook()->getDownloadStatus())
    {
        // 用buffer接受读取到的数据，通过m_tcpSocket的readAll接受
        QByteArray buffer = m_tcpSocket.readAll();
        // 简化命名使用
        Book *pBook = OpeWidget::getInstance().getBook();
        pBook->m_pFile.write(buffer);
        pBook->m_iRevice += buffer.size();

        if(pBook->m_iTotal == pBook->m_iRevice)
        {
            pBook->m_pFile.close();
            pBook->m_iTotal = 0;
            pBook->m_iRevice = 0;
            pBook->setDownloadStatus(false);
            QMessageBox::critical(this, "下载文件", "下载文件成功");
        }
        else if(pBook->m_iTotal < pBook->m_iRevice)
        {
            pBook->m_pFile.close();
            pBook->m_iTotal = 0;
            pBook->m_iRevice = 0;
            pBook->setDownloadStatus(false);
            QMessageBox::critical(this, "下载文件", "下载文件失败：传输的文件错误");
        }
        //其他情况代表数据还未下载完成
        return ;
    }

    //通信对象的总大小
    uint uiPDULen = 0;
    // 先获取通信对象的总大小
    m_tcpSocket.read((char*)&uiPDULen, sizeof(int));
    // 消息数据大小
    uint uiMsgLen = uiPDULen - sizeof(PDU);
    // 创建通信对象空间
    PDU *pdu = mkPDU(uiMsgLen);
    // 更新通信对象总大小
    pdu->uiPDULen = uiPDULen;
    // 读取剩余数据内容到通信对象中
    m_tcpSocket.read((char*)pdu + sizeof(int), uiPDULen - sizeof(int));
    switch(pdu->uiMsgType)
    {
    case ENUM_MSG_TYPE_REGIST_RESPOND:
    {
        regist(pdu);
        break;
    }
    case ENUM_MSG_TYPE_LOGIN_RESPOND:
    {
        login(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ALL_ONLINE_RESPOND:
    {
        OpeWidget::getInstance().getFriend()->showAllOnlineUser(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SEARCH_USER_RESPOND:
    {
        searchUser(pdu);
        break;
    }
    // 服务器转发请求
    case ENUM_MSG_TYPE_ADD_FRIEND_REQUEST:{
        addFriendRequest(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_RESPOND:{
        QMessageBox::information(this, "添加好友", pdu->caData);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_AGREE:{
        addFriendAgree(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ADD_FRIEND_REFUSE:{
        addFriendFefuse(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_FRIEND_RESPOND:{
        OpeWidget::getInstance().getFriend()->updateFriendList(pdu);
        break;
    }
    // 接受转发回来的消息（A想要删除B，转发给B的PDU）
    case ENUM_MSG_TYPE_DELETE_FRIEND_REQUEST:{
        deleteFriend(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DELETE_FRIEND_RESPOND: {
        QMessageBox::information(this, "删除好友", pdu->caData);
        break;
    }
    // 转发消息
    case ENUM_MSG_TYPE_PRIVATE_CHAT_REQUEST: {
        privateChat(pdu);
        break;
    }
    case ENUM_MSG_TYPE_GROUP_CHAT_REQUEST: {
        OpeWidget::getInstance().getFriend()->updateGroupMsg(pdu);
        break;
    }
    case ENUM_MSG_TYPE_CREATE_DIR_RESPOND:
    {
        createDir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_FLUSH_DIR_RESPOND:
    {
        flushDir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_DEL_DIR_RESPOND:
    {
        delDir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_RENAME_DIR_RESPOND:
    {
        renameDir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_ENTER_DIR_RESPOND:
    {
        enterDir(pdu);
        break;
    }
    case ENUM_MSG_TYPE_UPLOAD_FILE_RESPOND:
    {
        QMessageBox::information(this, "上传文件", pdu->caData);
        break;
    }
    case ENUM_MSG_TYPE_DEL_FILE_RESPOND:
    {
        QMessageBox::information(this, "删除文件", pdu->caData);
        break;
    }
    case ENUM_MSG_TYPE_DOWNLOAD_FILE_RESPOND:
    {
        qDebug() << pdu->caData;
        downloadFilePre(pdu);
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_RESPOND:
    {
        QMessageBox::information(this, "分享文件", pdu->caData);
        break;
    }
    case ENUM_MSG_TYPE_SHARE_FILE_NOTE:
    {
        shareFileNote(pdu);
        break;
    }
    case ENUM_MSG_TYPE_MOVE_FILE_RESPOND:
    {
        QMessageBox::information(this, "移动文件", pdu->caData);
        break;
    }
    default:
        break;
    }
    // 释放内存
    free(pdu);
    pdu = NULL;
}

void TcpClient::on_login_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    // 账号密码不为空时才能登录
    if(!strName.isEmpty() && !strPwd.isEmpty())
    {
        // 保存登录用户的名称
        m_strLoginName = strName;

        PDU *pdu = mkPDU(0);
        pdu->uiMsgType = ENUM_MSG_TYPE_LOGIN_REQUEST;
        // 前32个字符复制账号名
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);
        // 后32个字符复制密码
        strncpy(pdu->caData + 32, strPwd.toStdString().c_str(), 32);

        // 发送通信对象给服务器
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);

        // 释放内存
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "登录", "登录失败：用户名或密码不能为空");
    }
}

void TcpClient::on_regist_pb_clicked()
{
    QString strName = ui->name_le->text();
    QString strPwd = ui->pwd_le->text();
    // 账号密码不为空时才能注册
    if(!strName.isEmpty() && !strPwd.isEmpty())
    {
        PDU *pdu = mkPDU(0);// 没有用到消息类型，所以MsgLen给0

        pdu->uiMsgType = ENUM_MSG_TYPE_REGIST_REQUEST;

        // 将前端输入得到的用户名和密码用strncpy封装进PDU

        // 前32个字符复制账号名
        strncpy(pdu->caData, strName.toStdString().c_str(), 32);
        // 后32个字符复制密码
        strncpy(pdu->caData + 32, strPwd.toStdString().c_str(), 32);

        // 发送通信对象给服务器
        m_tcpSocket.write((char*)pdu, pdu->uiPDULen);

        // 释放内存
        free(pdu);
        pdu = NULL;
    }
    else
    {
        QMessageBox::critical(this, "注册", "注册失败：用户名或密码不能为空");
    }
}

void TcpClient::on_cancel_pb_clicked()
{
    // 链接服务器
    m_tcpSocket.connectToHost(QHostAddress(m_strIp), m_usPort);
}

void TcpClient::on_pwd_le_returnPressed()
{
    on_login_pb_clicked();
}
