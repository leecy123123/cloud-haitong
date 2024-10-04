#include "sharefile.h"
#include<QDebug>
#include<tcpclient.h>
#include<QStringList>

ShareFile::ShareFile(QWidget *parent) : QWidget(parent)
{
    m_pSelectAllPB = new QPushButton("全选");
    m_pCancleSelectPB = new QPushButton("取消选择");

    m_pOKPB = new QPushButton("确定");
    m_pCanclePB = new QPushButton("取消");

    m_pSA = new QScrollArea;
    m_pFriendW = new QWidget;
    m_pButtonGroup = new QButtonGroup(m_pFriendW);
    m_pButtonGroup->setExclusive(false);//设置可以多选
    // 这一行一定要放在 new QButtonGroup(m_pFriendW); 下面，否则按钮无法显示出来！！！！
    // 因为一个窗口不可用设置多个布局，且布局是有先后顺序的
    m_pFriendWVBL = new QVBoxLayout(m_pFriendW);

    // 把选择按钮和取消选择按钮水平排放
    QHBoxLayout *pTopHBL = new QHBoxLayout;
    pTopHBL->addWidget(m_pSelectAllPB);
    pTopHBL->addWidget(m_pCancleSelectPB);
    pTopHBL->addStretch();//弹簧

    // 把确定按钮和取消按钮水平排放
    QHBoxLayout *pDownHBL = new QHBoxLayout;
    pDownHBL->addWidget(m_pOKPB);
    pDownHBL->addWidget(m_pCanclePB);

    // 整体布局。选择的布局+展示好友的区域+确定/取消的布局
    QVBoxLayout *pMainVBL = new QVBoxLayout;
    pMainVBL->addLayout(pTopHBL);
    pMainVBL->addWidget(m_pSA);
    pMainVBL->addLayout(pDownHBL);
    setLayout(pMainVBL);


    //槽函数
    connect(m_pCancleSelectPB, SIGNAL(clicked(bool)),
            this, SLOT(cancelSelect()));
    connect(m_pSelectAllPB, SIGNAL(clicked(bool)),
            this, SLOT(selectAll()));
    connect(m_pOKPB, SIGNAL(clicked(bool)),
            this, SLOT(okShare()));
    connect(m_pCanclePB, SIGNAL(clicked(bool)),
            this, SLOT(cancelShare()));
}

ShareFile &ShareFile::getInstance()
{
    static ShareFile instance;
    return instance;
}

void ShareFile::updateFriend(QListWidget *pFriendList)
{
    if(NULL == pFriendList)
    {
        return ;
    }
    // 删除区域框中之前的内容
    QAbstractButton* temp = NULL;
    QList<QAbstractButton*> preFriendList = m_pButtonGroup->buttons();//获取之前的所有用户
    for(int i = 0; i < preFriendList.size(); i++)
    {
        temp = preFriendList[i];
        m_pFriendWVBL->removeWidget(temp);//移除掉之前的用户勾选框
        m_pButtonGroup->removeButton(temp);//移除掉之前的用户勾选框
        preFriendList.removeOne(temp);
        delete temp;
        temp = NULL;
    }
    // 向区域框中添加新的内容
    QCheckBox *pCB = NULL;
    for(int i = 0; i < pFriendList->count(); i++)
    {
        qDebug() << "好友的名称："
                << pFriendList->item(i)->text();
        pCB = new QCheckBox(pFriendList->item(i)->text());
        m_pFriendWVBL->addWidget(pCB);
        m_pButtonGroup->addButton(pCB);
    }
    m_pSA->setWidget(m_pFriendW);
}

void ShareFile::cancelSelect()
{
    QList<QAbstractButton*> cbList = m_pButtonGroup->buttons();//获取之前的所有用户
    for(int i = 0; i < cbList.count(); i++)
    {
        // 取消选中状态
        if(cbList[i]->isChecked())
        {
            cbList[i]->setChecked(false);
        }
    }
}

void ShareFile::selectAll()
{
    QList<QAbstractButton*> cbList = m_pButtonGroup->buttons();//获取之前的所有用户
    for(int i = 0; i < cbList.count(); i++)
    {
        // 设置选中状态
        if(!cbList[i]->isChecked())
        {
            cbList[i]->setChecked(true);
        }
    }
}

void ShareFile::okShare()
{
    QString strWantShareName = TcpClient::getInstance().loginName();//获得分享者
    QString strCurPath = TcpClient::getInstance().curPath();//获得当前路径
    QString strShareFileName = OpeWidget::getInstance().getBook()->getShareFileName();//获得共享的文件名字
    QString strPath = strCurPath + "/" + strShareFileName;//根据当前路径和分享的文件名进行拼接

    int num = 0;//选中的好友数
    QStringList friendNameList;
    friendNameList.clear();

    //获取选中的好友
    QList<QAbstractButton*> cbList = m_pButtonGroup->buttons();
    for(int i = 0; i < cbList.count(); i++)
    {
        // 如果是选中状态
        if(cbList[i]->isChecked())
        {
            num++;
            friendNameList.append(cbList[i]->text());
        }
    }

    // msg存放好友信息和文件路径
    PDU *pdu = mkPDU(32 * num + strPath.size() + 1);
    // data存放好友数量和当前用户名称
    pdu->uiMsgType = ENUM_MSG_TYPE_SHARE_FILE_REQUEST;
    sprintf(pdu->caData, "%s %d", strWantShareName.toStdString().c_str(), num);

    // for循环把人的名字放进caMsg
    for(int i = 0; i < num; i++)
    {
        memcpy((char*)pdu->caMsg + i*32
               , friendNameList.at(i).toStdString().c_str()
               , friendNameList.at(i).size());
    }

    // 将文件路径存放到PDU中的caMsg
    memcpy((char*)pdu->caMsg + num*32, strPath.toStdString().c_str(), strPath.size());
    TcpClient::getInstance().getTcpSocket().write((char*)pdu, pdu->uiPDULen);
    free(pdu);
    pdu = NULL;
}

void ShareFile::cancelShare()
{
    hide();
}
