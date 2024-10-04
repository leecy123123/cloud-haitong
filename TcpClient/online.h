#ifndef ONLINE_H
#define ONLINE_H

#include <QWidget>
#include"protocol.h"
#include<QString>

namespace Ui {
class Online;
}

class Online : public QWidget
{
    Q_OBJECT

public:
    explicit Online(QWidget *parent = 0);
    ~Online();
    // 展示所有用户
    void showUser(PDU *pdu);

private slots:
    void on_addFriend_Pb_clicked();

private:
    Ui::Online *ui;
};

#endif // ONLINE_H
