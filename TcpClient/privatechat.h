#ifndef PRIVATECHAT_H
#define PRIVATECHAT_H

#include <QWidget>
#include<QString>
#include"protocol.h"

namespace Ui {
class PrivateChat;
}

class PrivateChat : public QWidget
{
    Q_OBJECT

public:
    explicit PrivateChat(QWidget *parent = 0);
    ~PrivateChat();
    // 单例获取
    static PrivateChat& getInstace();
    // 设置聊天对象
    void setChatName(QString strName);
    // 更新消息页面
    void updateMsg(const PDU *pdu);

private slots:
    void on_pushButton_clicked();

private:
    Ui::PrivateChat *ui;
    QString m_strChatName;//聊天用户名称
    QString m_strLoginName;//登录者名称，在这里加一次是为了提高效率
};

#endif // PRIVATECHAT_H
