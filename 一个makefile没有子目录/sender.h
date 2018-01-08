#ifndef __SENDER__
#define __SENDER__
#include "comDef.h"

class receiver;

/* 消息发送器 */
class sender
{
public:
	sender();
	~sender();
	void msgDataPack(char* msgData, ...);  //将消息应答器类型，执行的函数类型及参数打包
	void sendMsg(receiver *rec, MSG_INFO *pMsgInfo); //发送消息到消息接收器
};

sender* getSenderInstance();
#endif

