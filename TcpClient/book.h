#ifndef BOOK_H
#define BOOK_H

#include <QWidget>
#include<QListWidgetItem>
#include<QPushButton>
#include<QVBoxLayout>
#include<QHBoxLayout>
#include"protocol.h"
#include<QTimer>
#include<QFile>

class Book : public QWidget
{
    Q_OBJECT
public:
    explicit Book(QWidget *parent = 0);
    void updateDirList(const PDU *pdu);
    QString getEnterPath();
    void setDownloadStatus(bool status);//设置下载状态
    bool getDownloadStatus();
    QString getSaveFilePath();
    void updateLocalDownloadFileName();//更新本地下载文件的文件名称
    QString getShareFileName();
    qint64 m_iTotal;//下载文件的总字节大小
    qint64 m_iRevice;//下载文件目前接受到文件的大小
    QFile m_pFile;//用于本地下载文件使用

signals:

public slots:
    void createDir();
    void flushDir();
    void delDir();
    void renameDir();
    void enterDir(const QModelIndex &index);
    // 返回上一级
    void returnPre();
    // 上传文件-预先发送消息
    void uploadPre();
    // 真实发送文件
    void uploadFileData();
    void delFile();
    void downloadFile();
    void shareFile();
    void moveFile();
    void selectTarDir();
private:
    QListWidget *m_pBookListW;
    QPushButton *m_pReturnPB;
    QPushButton *m_pCreateDirPB;
    QPushButton *m_pDelDirPB;
    QPushButton *m_pRenameDirPB;
    QPushButton *m_pFlushDirPB;
    QPushButton *m_pUploadFilePB;
    QPushButton *m_pDelFilePB;
    QPushButton *m_pDownloadFilePB;
    QPushButton *m_pShareFilePB;
    QPushButton *m_pMoveFilePB;
    QPushButton *m_pSelectTarDirPB;


    QString m_enterPath; //进入文件夹的路径
    QString m_strUploadFilePath;//上传文件的路径
    QTimer *m_pTimer;//定时器执行上传文件，【防止发送文件数据过快导致粘包】
    QString m_strSaveFilePath;//保存文件的路径
    bool m_pDownload;//是否处于文件下载状态
    QString m_shareFileName;//要分享的文件名字
    QString m_moveFileName;//要移动的文件名字
    QString m_moveFilePath;//要移动的文件路径
    QString m_moveTarDir;

};

#endif // BOOK_H
