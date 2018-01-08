#include "comDef.h"
#include "sender.h"
#include "receiver.h"
extern "C"{
	#include "log_out.h"
}
#include "mainApi.h"
using namespace std;

/************************
一、异步消息传递模型:
(1) 先将接收对象responser 注册到消息转发器(receiver)中，
     并设置每个responser 的优先级(low, mid, high)。
(2) 如果需要监听responser 执行完相应动作返回的结果，
      需要注册监听器到相应的responser。注意: 监听器
      listener 和应答器responser 要一一配对, listener开启线程
      开始监听。
(3) 通过sender发送接收的对象(responser)类型，执行的函
     数及函数所需要的参数给receiver。
(4) 消息转发器(receiver) 不断循环从队列中按优先级提
     取消息，并且根据消息的宿主(responser), 将该消息
     转发给对应的responser。
(5) responer 在注册到receiver 以后就开启线程在等待消息。

二、模型图:
	sender                 listener
	    |                        /\
	   \/                        |
	receiver                   |
	    |                        |
	   \/                        |
	responser            responser
*************************/

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
){
	if(resType < APL_RES_MIN || resType >= APL_RES_MAX){
		LOGOUT_ERROR("apl_registResponser() resType is invaild !!!!\n");
		return FALSE;
	}

	if(!pRes){
		LOGOUT_ERROR("apl_registResponser() pRes is NULL !!!!\n");
		return FALSE;
	}
		
	receiver* const rec_instance = getReceiverInstance(); //获得const指针，指针内容可变地址不可变
	if(rec_instance == NULL){
		LOGOUT_ERROR("apl_registResponser() get receiver pointer is NULL !!!!\n");
		return FALSE;
	}
	rec_instance->addResponser(resType, resPrior, pRes);
	return TRUE;
}

 /**
 * @brief function: 注销responser
 * @param[in] resType: responser 应答器标识
 * @return BOOL: 成功返回TRUE, 失败返回FALSE。
 */
 BOOL apl_unRegistResponser(APL_RES_TYPE resType)
 {
	 if(resType < APL_RES_MIN || resType >= APL_RES_MAX){
		 LOGOUT_ERROR("apl_unregistResponser() resType is invaild !!!!\n");
		 return FALSE;
	 }
  
	 receiver* const rec_instance = getReceiverInstance(); //获得const指针，指针内容可变地址不可变
	 if(rec_instance == NULL){
		 LOGOUT_ERROR("apl_unregistResponser() get receiver pointer is NULL !!!!\n");
		 return FALSE;
	 }
	 rec_instance->deleteResponser(resType);
	 return TRUE;
 }

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
 ){
	 if(resType < APL_RES_MIN || resType >= APL_RES_MAX){
		 LOGOUT_ERROR("apl_registListener() resType is invaild !!!!\n");
		 return FALSE;
	 }
 
	 if(!pRes || !pListener){
		 LOGOUT_ERROR("apl_registListener() pRes or pListener is NULL !!!!\n");
		 return FALSE;
	 }
		 
	 pRes->addListener(resType, pListener);
	 return TRUE;
 }

 /**
* @brief function: 注销listener 监听器
* @param[in] resType: 监听器对应的responser 应答器标识
* @param[in] pRes: 监听器对应的应答器指针
* @return BOOL: 成功返回TRUE, 失败返回FALSE。
*/
 BOOL apl_unRegistListener(
	  APL_RES_TYPE resType,    /* responser type: must in the range [0, APL_RES_MAX] */
	  responser* const pRes		 /* responser pointer */
  ){
	  if(resType < APL_RES_MIN || resType >= APL_RES_MAX){
		  LOGOUT_ERROR("apl_unregistListener() resType is invaild !!!!\n");
		  return FALSE;
	  }
  
	  if(!pRes){
		  LOGOUT_ERROR("apl_unregistListener() pRes  is NULL !!!!\n");
		  return FALSE;
	  }
		  
	  pRes->deleteListener(resType);
	  return TRUE;
  }

/**
* @brief function: 消息打包器
* @param[in] msgData: 用来存储打包的消息
* @return void.
*/
void apl_msgPacker(char* msgData, ...)
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

/**
* @brief function: 消息解包器
* @param[in] msgData: 用来存储打包的消息
* @return void.
*/
void apl_msgUnPacker(char* msgData, ...)
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
		memcpy(para, funParam, len);
		funParam += len;
		len = va_arg(vaList, int);
	}
	va_end(vaList);
	return;
}

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
){
	sender* const send_instance = getSenderInstance();
	if(send_instance == NULL){
		LOGOUT_ERROR("apl_sendMessage() send_instance is NULL\n");
		return FALSE;
	}

	 receiver* const rec_instance = getReceiverInstance(); //获得const指针，指针内容可变地址不可变
	 if(rec_instance == NULL){
		 LOGOUT_ERROR("apl_sendMessage() get receiver pointer is NULL !!!!\n");
		 return FALSE;
	 }

	MSG_INFO msgInfo;
	msgInfo.resType = resType;
	msgInfo.msg.eventID = eventID;
	memcpy(msgInfo.msg.msgData, msgData, sizeof(char) * 128);
	 
	send_instance->sendMsg(rec_instance, &msgInfo);
	return TRUE;
}

