#ifndef __MAIN_API_H__
#define __MAIN_API_H__
#include "responser.h"
#include "listener.h"

typedef enum enum_bool
{
    FALSE = 0,
    TRUE = 1
}BOOL;

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
