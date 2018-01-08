#include <iostream>
#include <stdarg.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <map>
#include <queue>
#include <pthread.h>
using namespace std;

class responser;

/* 应答器标识 */
typedef enum responser_type
{
	RES1 = 0,
	RES2,
	RES_MAX
}RES_TYPE;

/* 应答器优先级 */
typedef enum responser_priority
{
	LOW = 0,
	MID,
	HIGH,
	NULL_VALUE
}RES_PRIOR;

typedef struct msg
{
	int eventID;
	char msgData[128];
}EVENT_MSG;

typedef struct msgInfo
{
	RES_TYPE resType;
	EVENT_MSG msg;
}MSG_INFO;

/* 消息应答的父累 */
class responser
{
public:
	responser();
	~responser();
	virtual void onResponser() = 0;
	void msgDataUnPack(char* msgData, ...);
	void setEventMsg(const EVENT_MSG* srcMsg);

	EVENT_MSG eventMsg;
	pthread_cond_t	cond;
	pthread_mutex_t mutex;
};

responser::responser()
{
	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&cond, NULL);
	memset(&eventMsg, 0, sizeof(EVENT_MSG));
}

responser::~responser()
{
}

void responser::setEventMsg(const EVENT_MSG *srcMsg)
{
	memcpy(&eventMsg, srcMsg, sizeof(EVENT_MSG));
	return;
}

void responser::msgDataUnPack(char* msgData, ...)
{
	if(msgData == NULL)
		return;
	va_list vaList;
	void* para;
	char* funParam = msgData;
	va_start(vaList, funParam);
	//cout << "(1) msg un pack" << endl;
	int len = va_arg(vaList, int);
	while((len != 0) && (len != -1)){
		para = va_arg(vaList, void*);
		//cout << "(2) msg un pack len = " << len << endl;
		memcpy(para, funParam, len);
		//cout << "(3) msg un pack para = " << *(int*)para << endl;
		funParam += len;
		len = va_arg(vaList, int);
		//cout << "(4) msg un pack len = " << len << endl;
	}
	va_end(vaList);
	return;
}

/* 用户自己管理的应答器1 */
void* onResponser1Thread(void * arg);

class responser1: public responser
{
public:
	responser1();
	~responser1();
	/* fun1 和fun2 为各自应答器需要执行的函数 */
	void r1_fun1();
	int r1_fun2(int a, int b);
	virtual void onResponser();
private:
	pthread_t resThreadId;
};

responser1::responser1():resThreadId(0)
{
	pthread_create(&resThreadId, NULL, onResponser1Thread, (void*)this);
}

responser1::~responser1()
{
	resThreadId = 0;
}

void responser1::r1_fun1()
{

	cout << "r1_fun1() called!!!!!!!!" << endl;
}


int responser1::r1_fun2(int a, int b)
{

	cout << "r1_fun2() called!!!!!!!!" << endl;

	return a+ b;
}

void responser1::onResponser()
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		switch(eventMsg.eventID)
		{
			case 0:
				r1_fun1();
				break;
			case 1:
				int a, b;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &a,
					sizeof(int), &b, -1);
				cout << "The sum = " << r1_fun2(a, b) << endl;
				break;
			default:
				break;
		}
		
		pthread_mutex_unlock(&mutex);
	}
}

void* onResponser1Thread(void* arg)
{
	responser1* pRes1 = (responser1*)arg;
	pRes1->onResponser();
}


/* 应答器2 */
void* onResponser2Thread(void * arg);

class responser2: public responser
{
public:
	responser2();
	~responser2();
	double r2_fun1(double a, double b);
	int r2_fun2(int a, int b);
	virtual void onResponser();
private:
	pthread_t resThreadId;
};

responser2::responser2():resThreadId(0)
{
	pthread_create(&resThreadId, NULL, onResponser2Thread, (void*)this);
}

responser2::~responser2()
{
	resThreadId = 0;
}

double responser2::r2_fun1(double a, double b)
{
	cout << "r2_fun1() called!!!!!!!!" << endl;
	cout << " a * b = " << a * b << endl;
	return a*b;
}


int responser2::r2_fun2(int a, int b)
{

	cout << "r2_fun2() called!!!!!!!!" << endl;

	return a+ b;
}

void responser2::onResponser()
{
	while(1){
		pthread_mutex_lock(&mutex);
		pthread_cond_wait(&cond,&mutex);
		switch(eventMsg.eventID)
		{
			case 0:
				double a, b;
				msgDataUnPack(eventMsg.msgData, sizeof(double), &a,
					sizeof(double), &b, -1);
				r2_fun1(a, b);
				break;
			case 1:
				int c, d;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &c,
					sizeof(int), &d, -1);
				cout << "The sum is " << r2_fun2(c, d) << endl;
				break;
			default:
				break;
		}
		
		pthread_mutex_unlock(&mutex);
	}
}

void* onResponser2Thread(void* arg)
{
	responser2* pRes2 = (responser2*)arg;
	pRes2->onResponser();
}


/* 消息接收器(消息转发中心) */
void* onRecThread(void * arg);

class receiver
{
public:
	receiver();
	~receiver();
	void processMsg();  //根据消息的宿主(应答器) 将消息转发给应答器
	void addResponser(RES_TYPE resType, RES_PRIOR res_prior, responser* pRes); //注册应答器
	void deleteResponser(RES_TYPE resType); //删除应答器
	void addMsgIntoQue(MSG_INFO msgInfo);  //将消息插入等待队列
private:
	typedef std::map<RES_TYPE, responser*> RECE_MAP; //宿主类型及对象指针映射
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

int receiver::responserMaxNum = 10;

receiver::receiver():responserNum(0), recThreadId(0), priortyMap(NULL)
{
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&recThreadId, NULL, onRecThread, (void*)this);
	priortyMap = new RES_PRIOR [responserMaxNum];
	if(priortyMap != NULL){
		memset(priortyMap, 0, sizeof(RES_PRIOR) * responserMaxNum);
	}
}

receiver::~receiver(){
	delete [] priortyMap;
}

void receiver::addResponser(RES_TYPE resType, RES_PRIOR res_prior, responser* pRes)
{
	if(resType >= RES_MAX || !pRes)
		return;
	if(responserNum >= responserMaxNum)
		return;
	recMap.insert(make_pair(resType, pRes));
	priortyMap[resType] = res_prior;
	responserNum++;
	cout << "add responser success num = " << responserNum << endl;
}

void receiver::deleteResponser(RES_TYPE resType)
{
	RECE_MAP::iterator ite;
	ite = recMap.find(resType);
	if(ite != recMap.end()){
		recMap.erase(ite);
	}else{
		cout << "in the recmap can not find responser: "<< resType << endl;
	}
}

void receiver::addMsgIntoQue(MSG_INFO msgInfo)
{
	pthread_mutex_lock(&mutex);
	switch(priortyMap[msgInfo.resType])
	{
		case HIGH:
			recHighQueue.push(msgInfo);
			break;
		case MID:
			recMidQueue.push(msgInfo);
			break;
		case LOW:
			recLowQueue.push(msgInfo);
			break;
		default:
			recMidQueue.push(msgInfo);
			break;
	}
	pthread_mutex_unlock(&mutex);
}

void receiver::processMsg()
{
	while(1){
		std::queue<MSG_INFO> *recQueue = NULL;
		while(1){
			if(!recHighQueue.empty()){
				recQueue = &recHighQueue;
				break;
			}
			else if(!recMidQueue.empty()){
				recQueue = &recMidQueue;
				break;
			}
			else{
				recQueue = &recLowQueue;
				break;
			}
		}
		
		if(!recQueue->empty()){	
			pthread_mutex_lock(&mutex);
			MSG_INFO pMsgInfo = recQueue->front();
			RECE_MAP::iterator ite = recMap.begin();
			for(; ite != recMap.end(); ite++){
				if(ite->first == pMsgInfo.resType)
					break;
			}
			if(ite == recMap.end()) continue;

			responser* pRes = ite->second;
			pRes->setEventMsg(&pMsgInfo.msg);
		       pthread_cond_signal(&pRes->cond);
			pthread_mutex_unlock(&mutex);
			recQueue->pop();
		}
	}
}

void* onRecThread(void* arg)
{
	receiver* pRec = (receiver*)arg;
	pRec->processMsg();
}



/* 消息发送器 */
class sender
{
public:
	sender();
	~sender();
	void msgDataPack(char* msgData, ...);  //将消息应答器类型，执行的函数类型及参数打包
	void sendMsg(receiver *rec, MSG_INFO *pMsgInfo); //发送消息到消息接收器
};

sender::sender()
{
}

sender::~sender()
{
}

void sender::sendMsg(receiver * pRec, MSG_INFO *pMsgInfo)
{
	if(pRec == NULL || pMsgInfo == NULL)
		return;
	pRec->addMsgIntoQue(*pMsgInfo);
}

void sender::msgDataPack(char* msgData, ...)
{
	if(msgData == NULL)
		return;
	va_list vaList;
	void* para;
	char* funParam = msgData;
	va_start(vaList, funParam);
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


static receiver rec_instance;
static responser1 res1_instance;
static responser2 res2_instance;
static sender send_instance;

 int main(void)
 {
      rec_instance.addResponser(RES1, MID, &res1_instance);
      rec_instance.addResponser(RES2, HIGH, &res2_instance);
 	while(1){
		 MSG_INFO msgInfo;
#if 1
		 msgInfo.resType = RES1;
		 msgInfo.msg.eventID = 0;
		 send_instance.sendMsg(&rec_instance, &msgInfo);
#endif

#if 1
		 msgInfo.msg.eventID = 1;
		 int a = 10, b = 122;
		 send_instance.msgDataPack((char*)msgInfo.msg.msgData,
		 	sizeof(int), &a,
		 	sizeof(int), &b, -1);
		 send_instance.sendMsg(&rec_instance, &msgInfo);
#endif

#if 1
		 msgInfo.resType = RES2;
		 msgInfo.msg.eventID = 0;
		 double c = 4.23, d = 2.32;
		 send_instance.msgDataPack(msgInfo.msg.msgData,
		 	sizeof(double), &c,
		 	sizeof(double), &d, -1);
		 send_instance.sendMsg(&rec_instance, &msgInfo);
#endif

		 sleep(1);
 	}
 } 

