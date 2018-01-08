#ifndef __APL_MY_LISTENER__
#define __APL_MY_LISTENER__
#include "comDef.h"

/* 监听器的父类 */
class listener
{
public:
	listener();
	~listener();
	virtual void onListener() = 0;  //纯虚函数使 responser 不能实例化
	void addEventMsgIntoQue(EVENT_MSG event_msg);
       void clearEventMsgQue();
	void msgDataUnPack(char* msgData, ...);
       pthread_cond_t* getSignalCond();
protected:
	pthread_cond_t	cond;
	pthread_mutex_t mutex;
       std::queue<EVENT_MSG>  lisQueue;
};
#endif

