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
	void processMsg();  //根据消息的宿主(应答器) 将消息转发给应答器
	void addResponser(RES_TYPE resType, RES_PRIOR res_prior, responser* const pRes); //注册应答器
	void deleteResponser(RES_TYPE resType); //删除应答器
	void addMsgIntoQue(MSG_INFO msgInfo);  //将消息插入等待队列
private:
	typedef std::map<RES_TYPE, responser* const> RECE_MAP; //宿主类型及对象指针映射
	RECE_MAP recMap;   
	RES_PRIOR *priortyMap;   //存储应答器的优先级
	std::queue<MSG_INFO> recHighQueue;   // 消息队列-- 优先级高
	std::queue<MSG_INFO> recMidQueue;   // 消息队列-- 优先级普通
	std::queue<MSG_INFO> recLowQueue;   // 消息队列-- 优先级低
	pthread_t recThreadId;
	pthread_mutex_t mutex;
	static int responserMaxNum;  //最大能容纳的应答器个数
	int responserNum;
};

receiver* getReceiverInstance();
#endif
