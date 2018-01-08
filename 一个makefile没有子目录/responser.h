//#ifndef __APL_MY_RESPONSER__
//#define __APL_MY_RESPONSER__
#include "comDef.h"

/* 消息应答的父类 */
class responser
{
public:
	responser();
	~responser();
	virtual void onResponser() = 0;  //纯虚函数使 responser 不能实例化
	void msgDataUnPack(char* msgData, ...);
	void setEventMsg(const EVENT_MSG* srcMsg);
       pthread_cond_t* getPthreadCond();
protected:
	EVENT_MSG eventMsg;
	pthread_cond_t	cond;
	pthread_mutex_t mutex;
};
//#endif

