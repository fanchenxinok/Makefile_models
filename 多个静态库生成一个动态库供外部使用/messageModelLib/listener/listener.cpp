#include "listener.h"

/* 消息应答的父累 */
listener::listener()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);

	clearEventMsgQue();
}

listener::~listener()
{
}

pthread_cond_t* listener::getSignalCond()
{
	return &cond;
}

void listener::msgDataUnPack(char* msgData, ...)
{
	if(msgData == NULL)
		return;
	va_list vaList;
	void* para;
	char* funParam = msgData;
	va_start(vaList, msgData);
	//cout << "(1) msg un pack" << endl;
	int len = va_arg(vaList, int);
	while((len != 0) && (len != -1)){
		para = va_arg(vaList, void*);
		//cout << "(2) msg un pack len = " << len << endl;
		memcpy(para, funParam, len);
		//cout << "(3) msg un pack para = " << *(int*)para << endl;
		funParam += len;
		len = va_arg(vaList, int);
		//cout << "(4) msg un pack len = " << len << endl;
	}
	va_end(vaList);
	return;
}

void listener::addEventMsgIntoQue(EVENT_MSG event_msg)
{
	lisQueue.push(event_msg);
	return;
}

void listener::clearEventMsgQue()
{
	while(!lisQueue.empty())
		lisQueue.pop();
	return;
}


