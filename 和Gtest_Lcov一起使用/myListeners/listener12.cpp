#include "listener12.h"
extern "C"{
	#include "log_out.h"
}

using namespace std;

/* 用户自己管理的应答器1 */
listener1::listener1():lisThreadId(0)
{
	pthread_create(&lisThreadId, NULL, onListener1Thread, (void*)this);
	pthread_setname_np(lisThreadId, "listener1");
}

listener1::~listener1()
{
	lisThreadId = 0;
}

void listener1::l1_fun1()
{
	LOGOUT_WARNING("%%%%Listener 1 receive message: l1_fun1() called!!!!!!!!");
	return;
}


void listener1::l1_fun2(int sum)
{
	LOGOUT_WARNING("%%%%%%%%l1_fun2() called sum = %d !!!!!!\n", sum);
	return;
}

void listener1::onListener()
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		EVENT_MSG eventMsg = lisQueue.front();
		switch(eventMsg.eventID)
		{
			case L1_FUN1:
			{
				l1_fun1();
				MY_INFO *myinfo = new MY_INFO();
				msgDataUnPack(eventMsg.msgData, sizeof(MY_INFO), myinfo, -1);
				//cout << "name : " << myinfo.name;
				LOGOUT_COLOR(WHITE, GREEN, "name:%s; age:%d; sex:%c\n",
					myinfo->name.c_str(), myinfo->age, myinfo->sex);
				break;
			}
			case L1_FUN2:
			{
				int sum;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &sum, -1);
				l1_fun2(sum);
				break;
			}
			default:
				break;
		}
		lisQueue.pop();
		pthread_mutex_unlock(&mutex);
	}
}

void* onListener1Thread(void* arg)
{
	listener1* pLis1 = (listener1*)arg;
	pLis1->onListener();
}

/* 应答器2 */
listener2::listener2():lisThreadId(0)
{
	pthread_create(&lisThreadId, NULL, onListener2Thread, (void*)this);
	pthread_setname_np(lisThreadId, "listener2");
}

listener2::~listener2()
{
	lisThreadId = 0;
}

void listener2::l2_fun1(double res)
{
	LOGOUT_NOTICE("%%%%%%%%%%l2_fun1() called!!!!!!!! res:a*b = %f\n", res);
	return;
}


void listener2::l2_fun2(int sum)
{
	LOGOUT_NOTICE("%%%%%%%%l2_fun2() called sum = %d !!!!!!!\n", sum);
	return;
}

void listener2::onListener()
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		EVENT_MSG eventMsg = lisQueue.front();
		switch(eventMsg.eventID)
		{
			case L2_FUN1:
				double res;
				msgDataUnPack(eventMsg.msgData, sizeof(double), &res, -1);
				l2_fun1(res);
				break;
			case L2_FUN2:
				int sum;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &sum, -1);
				l2_fun2(sum);
				break;
			default:
				break;
		}
		lisQueue.pop();
		pthread_mutex_unlock(&mutex);
	}
}

void* onListener2Thread(void* arg)
{
	listener2* pLis2 = (listener2*)arg;
	pLis2->onListener();
}

