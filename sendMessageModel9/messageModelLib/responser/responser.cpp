#include "responser.h"
#include "listener.h"

LIS_MAP responser::lisMap;

void* onResponserThread(void* arg)
{
	responser* pRes = (responser*)arg;
	pRes->onResponser();
}

/* 消息应答的父累 */
responser::responser() : resThreadId(NULL)
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	memset(&eventMsg, 0, sizeof(EVENT_MSG));

	pthread_create(&resThreadId, NULL, onResponserThread, (void*)this);
}

responser::responser(const char* pthreadName) : resThreadId(NULL)
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	memset(&eventMsg, 0, sizeof(EVENT_MSG));

	pthread_create(&resThreadId, NULL, onResponserThread, (void*)this);
	if(pthreadName != NULL){
		pthread_setname_np(resThreadId, pthreadName);
	}
}

responser::~responser()
{
}

void responser::setEventMsg(const EVENT_MSG *srcMsg)
{
	pthread_mutex_lock(&mutex);
	memcpy(&eventMsg, srcMsg, sizeof(EVENT_MSG));
	pthread_mutex_unlock(&mutex);
	return;
}

pthread_cond_t* responser::getPthreadCond()
{
	return &cond;
}

void responser::msgDataPack(char* msgData, ...)
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


void responser::msgDataUnPack(char* msgData, ...)
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

void responser::addListener(RES_TYPE resType, listener* const pListener)
{
	if(!pListener || resType >= RES_MAX)
		return;
	lisMap.insert(make_pair(resType, pListener));
}

void responser::deleteListener(RES_TYPE resType)
{
	if(resType >= RES_MAX)
		return;
	LIS_MAP::iterator ite;
	ite = lisMap.find(resType);
	if(ite != lisMap.end()){
		lisMap.erase(ite);
		cout << "delete Listener of : " << resType << " success !!!" << endl;
	}else{
		cout << "in the listener map can not find listener: "<< resType << endl;
	}
}

void responser::sendMsgToListener(MSG_INFO msgInfo)
{
	if(msgInfo.resType >= RES_MAX)
		return;
	LIS_MAP::iterator ite;
	ite = lisMap.find(msgInfo.resType);
	if(ite != lisMap.end()){
		listener* const pListener = ite->second;
		pListener->addEventMsgIntoQue(msgInfo.msg);
		pthread_mutex_lock(&listener_mutex);
		pthread_cond_signal(pListener->getSignalCond());
		pthread_mutex_unlock(&listener_mutex);
	}
	return;
}

