#include "comDef.h"
#include "sender.h"
#include "receiver.h"
#include "responser12.h"
using namespace std;
/************************
异步消息传递模型:
(1) 先将接收对象responser 注册到消息转发器(receiver)中，
     并设置每个responser 的优先级(low, mid, high)。
(2) 通过sender发送接收的对象(responser)类型，执行的函
     数及函数所需要的参数。
(3) 消息转发器(receiver) 不断循环从队列中按优先级提
     取消息，并且根据消息的宿主(responser), 将该消息
     转发给对应的responser。
(4) responer 在注册到receiver 以后就开启线程在等待消息。
*************************/

static responser1 res1_instance;
static responser2 res2_instance;

 int main(void)
 {
      receiver* const rec_instance = getReceiverInstance(); //获得const指针，指针内容可变地址不可变
      sender* const send_instance = getSenderInstance();
      rec_instance->addResponser(RES1, MID, &res1_instance);
      rec_instance->addResponser(RES2, HIGH, &res2_instance);
 	while(1){
		 MSG_INFO msgInfo;

		 msgInfo.resType = RES1;  //设置动作对象
		 msgInfo.msg.eventID = R1_FUN1; //设置应该做什么动作
		 send_instance->sendMsg(rec_instance, &msgInfo); //发送开始消息及动作参数

		 //rec_instance->deleteResponser(RES1);

		 msgInfo.msg.eventID = R1_FUN2;
		 int a = 10, b = 122;
		 send_instance->msgDataPack((char*)msgInfo.msg.msgData,
		 	sizeof(int), &a,
		 	sizeof(int), &b, -1);
		 send_instance->sendMsg(rec_instance, &msgInfo);

		 msgInfo.resType = RES2;
		 msgInfo.msg.eventID = R2_FUN1;
		 double c = 4.23, d = 2.32;
		 send_instance->msgDataPack(msgInfo.msg.msgData,
		 	sizeof(double), &c,
		 	sizeof(double), &d, -1);
		 send_instance->sendMsg(rec_instance, &msgInfo);

		 msgInfo.msg.eventID = R2_FUN2;
		 a = 20;
		 b = 30;
		 send_instance->msgDataPack(msgInfo.msg.msgData,
		 	sizeof(int), &a,
		 	sizeof(int), &b, -1);
		 send_instance->sendMsg(rec_instance, &msgInfo);

		 sleep(1);
 	}
 } 

