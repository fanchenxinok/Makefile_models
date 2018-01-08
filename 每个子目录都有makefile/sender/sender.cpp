#include "sender.h"
#include "receiver.h"

/* ÏûÏ¢·¢ËÍÆ÷ */
static sender s_sender_instance;

sender* getSenderInstance()
{
	return &s_sender_instance;
}

sender::sender()
{
}

sender::~sender()
{
}

void sender::sendMsg(receiver * pRec, MSG_INFO *pMsgInfo)
{
	if(pRec == NULL || pMsgInfo == NULL)
		return;
	pRec->addMsgIntoQue(*pMsgInfo);
}

void sender::msgDataPack(char* msgData, ...)
{
	if(msgData == NULL)
		return;
	va_list vaList;
	void* para;
	char* funParam = msgData;
	va_start(vaList, msgData);
	int len = va_arg(vaList, int);
	while((len != 0) && (len != -1)){
		para = va_arg(vaList, void*);
		memcpy(funParam, para, len);
		funParam += len;
		len = va_arg(vaList, int);
	}
	va_end(vaList);
	return;
}

