#ifndef __MAIN_API_H__
#define __MAIN_API_H__
#include <iostream>
#include <queue>
#include <map>
#include <pthread.h>
using namespace std;

typedef enum enum_bool
{
    FALSE = 0,
    TRUE = 1
}BOOL;

typedef unsigned short RES_TYPE;
typedef unsigned short APL_RES_TYPE;

typedef enum
{
    RES_LOW = 0,
    RES_MID,
    RES_HIGH
}APL_RES_PRIOR;

enum
{
    APL_RES_MIN = 0,
    APL_RES_MAX = 10
};

typedef struct msg
{
	int eventID;
       int dataSize;
	char msgData[128];
}EVENT_MSG;

typedef struct msgInfo
{
	RES_TYPE resType;
	EVENT_MSG msg;
}MSG_INFO;


/* 监听器的父类 */
class listener
{
public:
	listener();
       listener(const char* pthreadName);
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

       pthread_t lisThreadId;
};

typedef std::map<RES_TYPE, listener* const>  LIS_MAP;

/* 消息应答的父类 */
class responser
{
public:
	responser();
       responser(const char* pthreadName);
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

       pthread_t resThreadId;
};


/**
* @brief function: 注册responser 到消息转发中心
* @param[in] resType: responser 应答器标识
* @param[in] resPrior: responser 应答器接收消息的优先级
* @param[in] pRes: 需要注册的responser 应答器指针。
* @return BOOL: 成功返回TRUE, 失败返回FALSE。
*/
BOOL apl_registResponser(
	APL_RES_TYPE resType,    /* responser type: must in the range [0, APL_RES_MAX] */
	APL_RES_PRIOR resPrior,  /* priority: must be the value RES_LOW, RES_MID, RES_HIGH */
	responser* const pRes    /* responser pointer */
);

 /**
 * @brief function: 注销responser
 * @param[in] resType: responser 应答器标识
 * @return BOOL: 成功返回TRUE, 失败返回FALSE。
 */
 BOOL apl_unRegistResponser(APL_RES_TYPE resType);


/**
* @brief function: 注册listener 监听器 (一个应答器对应一个监听器)
* @param[in] resType: 监听器对应的responser 应答器标识
* @param[in] pRes: 监听器对应的应答器指针
* @param[in] pListener: 需要注册的listener 监听器指针。
* @return BOOL: 成功返回TRUE, 失败返回FALSE。
*/
BOOL apl_registListener(
	 APL_RES_TYPE resType,	  /* responser type: must in the range [0, APL_RES_MAX] */
	 responser* const pRes,         /* responser pointer */
	 listener* const pListener	  /* listener pointer */
 );

 /**
* @brief function: 注销listener 监听器
* @param[in] resType: 监听器对应的responser 应答器标识
* @param[in] pRes: 监听器对应的应答器指针
* @return BOOL: 成功返回TRUE, 失败返回FALSE。
*/
 BOOL apl_unRegistListener(
	  APL_RES_TYPE resType,    /* responser type: must in the range [0, APL_RES_MAX] */
	  responser* const pRes		 /* responser pointer */
  );

/**
* @brief function: 消息打包器
* @param[in] msgData: 用来存储打包的消息
* @return void.
*/
void apl_msgPacker(char* msgData, ...);


/**
* @brief function: 消息解包器
* @param[in] msgData: 用来存储打包的消息
* @return void.
*/
void apl_msgUnPacker(char* msgData, ...);

/**
* @brief function: 发送消息命令
* @param[in] resType: 应答器标识
* @param[in] eventID: 应答器执行的event id.
* @param[in] msgData: 需要发送的消息
* @return BOOL: 成功返回TRUE, 否则返回FALSE.
*/
BOOL apl_sendMessage(
	APL_RES_TYPE resType,
	int eventID,
	char* msgData
);
#endif
