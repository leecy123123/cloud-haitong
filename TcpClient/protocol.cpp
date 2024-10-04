#include"protocol.h"

PDU *mkPDU(uint uiMsgLen)
{
    // 通信对象的总大小 = 通信对象基础字段大小 + 消息数据大小
    uint uiPDULen = sizeof(PDU) + uiMsgLen;
    // 创建通信对象内存空间
    PDU *pdu = (PDU*)malloc(uiPDULen);
    // 如果创建失败，则结束程序
    if(NULL == pdu)
    {
        exit(EXIT_FAILURE);
    }
    // 初始化内存中的数据
    memset(pdu, 0, uiPDULen);
    // 设置通信对象总大小和消息大小
    pdu->uiPDULen = uiPDULen;
    pdu->uiMsgLen = uiMsgLen;
    // 返回创建的通信对象地址
    return pdu;
}
