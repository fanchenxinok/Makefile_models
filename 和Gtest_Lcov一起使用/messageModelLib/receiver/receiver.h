#ifndef __RECEIVER__
#define __RECEIVER__
#include "comDef.h"

class responser;
/* 消息接收器(消息转发中心) */
class receiver
{
public:
	receiver();
	~receiver();
	void processResMsg();  //根据消息的宿主(应答器) 将消息转发给应答器
	void addResponser(RES_TYPE resType, RES_PRIOR res_prior, responser* const pRes); //注册应答器
	void deleteResponser(RES_TYPE resType); //删除应答器
	void addMsgIntoResQue(MSG_INFO msgInfo);  //将消息插入等待队列

private:
	typedef std::map<RES_TYPE, responser* const> RES_MAP; //宿主类型及对象指针映射
	RES_MAP resMap;   
	RES_PRIOR *priortyMap;   //存储应答器的优先级
	#if 0
	std::queue<MSG_INFO> resHighQueue;   // 消息队列-- 优先级高
	std::queue<MSG_INFO> resMidQueue;   // 消息队列-- 优先级普通
	std::queue<MSG_INFO> resLowQueue;   // 消息队列-- 优先级低
       #else
       typedef std::queue<MSG_INFO> RES_QUE;
       RES_QUE resQueues[HIGH+1];
       #endif

	pthread_t recThreadId;
	pthread_mutex_t mutex;
	static int responserMaxNum;  //最大能容纳的应答器个数
	int responserNum;
};

receiver* getReceiverInstance();
#endif
