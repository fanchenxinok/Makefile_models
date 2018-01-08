#include "listener12.h"
extern "C"{
	#include "log_out.h"
}

using namespace std;

/* 用户自己管理的应答器1 */
listener1::listener1() : listener("listener1")
{
	//pthread_create(&lisThreadId, NULL, onListener1Thread, (void*)this);
	//pthread_setname_np(lisThreadId, "listener1");
}

listener1::listener1(const char* pthreadName) :
	listener((pthreadName == NULL) ? "listener1" : pthreadName)
{
}

listener1::~listener1()
{
	//lisThreadId = 0;
}

void listener1::l1_fun1()
{
	LOGOUT_COLOR(RED,YELLOW,"%%%%Listener 1 receive message: l1_fun1() called!!!!!!!!");
	return;
}


void listener1::l1_fun2(int sum)
{
	LOGOUT_COLOR(RED,YELLOW,"%%%%%%%%l1_fun2() called sum = %d !!!!!!\n", sum);
	return;
}

void listener1::onListener()
{
	MY_INFO *myinfo = new MY_INFO();
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		EVENT_MSG eventMsg = lisQueue.front();
		switch(eventMsg.eventID)
		{
			case L1_FUN1:
			{
				l1_fun1();
				#if USE_PROTO_BUF
				student st1;
				st1.ParseFromArray(eventMsg.msgData, eventMsg.dataSize);  //序列化 方式1
				//st1.ParseFromArray((void *)str.c_str(), str.size());  //序列化 方式2
				cout << "name: " << st1.base_info().name() << endl;
				cout << "age: " << st1.base_info().age() << endl;
				cout << "sex: " << st1.base_info().sex() << endl;
				cout << "student id: " << st1.student_id() << endl;

				cout << "parents number: " << st1.parents_size() << endl;
				for(int i = 0; i < st1.parents_size(); i++){
					cout << "<" << i << ">:" << endl;
					cout << "name: " << st1.parents(i).name() << endl;
					cout << "age: " << st1.parents(i).age() << endl;
					cout << "sex: " << st1.parents(i).sex() << endl;
				}
				#else
				msgDataUnPack(eventMsg.msgData, sizeof(MY_INFO), myinfo, -1);
				//cout << "name : " << myinfo.name;
				LOGOUT_COLOR(WHITE, GREEN, "name:%s; age:%d; sex:%c\n",
					myinfo->name.c_str(), myinfo->age, myinfo->sex);
				#endif
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
	delete myinfo;
	myinfo = NULL;
}

/*	void* onListener1Thread(void* arg)
{
	listener1* pLis1 = (listener1*)arg;
	pLis1->onListener();
}*/	

/* 应答器2 */
listener2::listener2() : listener("listener2")
{
	//pthread_create(&lisThreadId, NULL, onListener2Thread, (void*)this);
	//pthread_setname_np(lisThreadId, "listener2");
}

listener2::listener2(const char* pthreadName) :
	listener((pthreadName == NULL) ? "listener2" : pthreadName)
{
}


listener2::~listener2()
{
	//lisThreadId = 0;
}

void listener2::l2_fun1(double res)
{
	LOGOUT_COLOR(BLACK,YELLOW,"%%%%%%%%%%l2_fun1() called!!!!!!!! res:a*b = %f\n", res);
	return;
}


void listener2::l2_fun2(int sum)
{
	LOGOUT_COLOR(BLACK,YELLOW,"%%%%%%%%l2_fun2() called sum = %d !!!!!!!\n", sum);
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

/*	void* onListener2Thread(void* arg)
{
	listener2* pLis2 = (listener2*)arg;
	pLis2->onListener();
}*/	

