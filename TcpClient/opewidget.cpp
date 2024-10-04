#include "opewidget.h"

OpeWidget::OpeWidget(QWidget *parent) : QWidget(parent)
{
    // 实例化出这两个按钮
    m_pFriend = new Friend;
    m_pBook = new Book;

    // 实例化出这个堆栈窗口，一次只会显示一个（好友/图书）
    m_pSW = new QStackedWidget;

    // 创建
    m_pSW->addWidget(m_pFriend);
    m_pSW->addWidget(m_pBook);

    m_pListW = new QListWidget(this);

    // 把创建出来的这两个窗口放进堆栈
    m_pListW->addItem("好友");
    m_pListW->addItem("图书");

    // 显示界面
    QHBoxLayout *pMain = new QHBoxLayout; // 水平布局

    // 列表和堆栈窗口都放进去
    pMain->addWidget(m_pListW);
    pMain->addWidget(m_pSW);
    setLayout(pMain);

    // 添加信号
    // tab的切换
    connect(m_pListW, SIGNAL(currentRowChanged(int)), m_pSW, SLOT(setCurrentIndex(int)));
}

OpeWidget &OpeWidget::getInstance()
{
    static OpeWidget instance;
    return instance;
}

Friend *OpeWidget::getFriend()
{
    return m_pFriend;
}

Book *OpeWidget::getBook()
{
    return m_pBook;
}
