#include "myComDef.h"
#include "responser12.h"
#include "listener12.h"

extern "C" {
	#include "log_out.h"
}

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

static responser1 res1_instance;
static responser2 res2_instance;

static listener1 lis1_instance;
static listener2 lis2_instance;

 int main(void)
 {
       /* 注册应答器到receiver */
	BOOL res = FALSE;
	res = apl_registResponser(RES1, RES_MID, &res1_instance);
	if(res == TRUE){
		LOGOUT_NOTICE("regist responser 1 success !!!!!!");
	}else{
		LOGOUT_ERROR("regist responser 1 fail !!!!!!");
	}
	
	res = apl_registResponser(RES2, RES_HIGH, &res2_instance);
	if(res == TRUE){
		LOGOUT_NOTICE("regist responser 2 success !!!!!!");
	}else{
		LOGOUT_ERROR("regist responser 2 fail !!!!!!");
	}
	

       /* 监听器注册 */
	apl_registListener(RES1, &res1_instance, &lis1_instance);
	apl_registListener(RES2, &res2_instance, &lis2_instance);
 	while(1){
		 MSG_INFO msgInfo;
		 char msgData[128];

		 msgInfo.resType = RES1;  //设置动作对象
		 msgInfo.msg.eventID = R1_FUN1; //设置应该做什么动作
		 apl_sendMessage(RES1, R1_FUN1, msgData); //发送开始消息及动作参数

		 //apl_unRegistResponser(RES1);

		 msgInfo.msg.eventID = R1_FUN2;
		 int a = 10, b = 122;
		 apl_msgPacker(msgData,
		 	sizeof(int), &a,
		 	sizeof(int), &b, -1);
		 apl_sendMessage(RES1, R1_FUN2, msgData);

		 msgInfo.resType = RES2;
		 msgInfo.msg.eventID = R2_FUN1;
		 double c = 4.23, d = 2.32;
		 apl_msgPacker(msgData,
		 	sizeof(double), &c,
		 	sizeof(double), &d, -1);
		 apl_sendMessage(RES2, R2_FUN1, msgData);

		 msgInfo.msg.eventID = R2_FUN2;
		 a = 20;
		 b = 30;
		 apl_msgPacker(msgData,
		 	sizeof(int), &a,
		 	sizeof(int), &b, -1);
		 apl_sendMessage(RES2, R2_FUN2, msgData);

		 sleep(1);
 	}
	return 0;
 } 

