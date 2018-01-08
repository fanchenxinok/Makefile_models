#ifndef __APL_MY_RESPONSER__
#define __APL_MY_RESPONSER__
#include "comDef.h"
class listener;

typedef std::map<RES_TYPE, listener* const>  LIS_MAP;

/* 消息应答的父类 */
class responser
{
public:
	responser();
	~responser();
	virtual void onResponser() = 0;  //纯虚函数使 responser 不能实例化
	void msgDataUnPack(char* msgData, ...);
       void msgDataPack(char* msgData, ...);
	void setEventMsg(const EVENT_MSG* srcMsg);
       pthread_cond_t* getPthreadCond();

       void sendMsgToListener(MSG_INFO msgInfo);  //发送消息给监听器
	void addListener(RES_TYPE resType, listener* const pListener); //注册监听器
	void deleteListener(RES_TYPE resType); //删除监听器

protected:
	EVENT_MSG eventMsg;
	pthread_cond_t	cond;
	pthread_mutex_t mutex;

       static LIS_MAP lisMap;
       pthread_mutex_t listener_mutex;
};
#endif

