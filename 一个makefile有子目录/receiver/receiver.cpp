#include "receiver.h"
#include "responser.h"

using namespace std;

/* 消息接收器(消息转发中心) */
static receiver s_receiver_instance;
receiver *getReceiverInstance()
{
	return &s_receiver_instance;
}

void* onRecThread(void * arg);

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

void receiver::addResponser(
RES_TYPE resType, 
RES_PRIOR res_prior,
responser* const pRes  //const指针地址不可变，内容可变
){
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
		cout << "delete Responser : " << resType << " success !!!" << endl;
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

void mutex_clean(void* mutex)
{
	pthread_mutex_unlock((pthread_mutex_t*)mutex);
}

void receiver::processMsg()
{
	while(1){
		std::queue<MSG_INFO> *recQueue = NULL;
		while(1){
			//cout << "loop get message!!!!" << endl;
			if(!recHighQueue.empty()){
				recQueue = &recHighQueue;
				//cout << "receiver high queue have data!!!!" << endl;
				break;
			}
			else if(!recMidQueue.empty()){
				recQueue = &recMidQueue;
				//cout << "receiver mid queue have data!!!!" << endl;
				break;
			}
			else if(!recLowQueue.empty()){
				recQueue = &recLowQueue;
				//cout << "receiver low queue have data!!!!" << endl;
				break;
			}
		}
		
		if(!recQueue->empty()){
			int old_cancel;
			pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_cancel);
			pthread_cleanup_push(mutex_clean, &mutex);
			pthread_mutex_lock(&mutex);
			MSG_INFO pMsgInfo = recQueue->front();
			RECE_MAP::iterator ite = recMap.begin();
			for(; ite != recMap.end(); ite++){
				if(ite->first == pMsgInfo.resType)
					break;
			}
			if(ite == recMap.end()) continue;

			responser* const pRes = ite->second;
			pRes->setEventMsg(&pMsgInfo.msg);
		       pthread_cond_signal(pRes->getPthreadCond());
			recQueue->pop();
			pthread_mutex_unlock(&mutex);
			pthread_cleanup_pop(0);
			pthread_setcanceltype(old_cancel, NULL);
		}
	}
}

void* onRecThread(void* arg)
{
	receiver* pRec = (receiver*)arg;
	pRec->processMsg();
}
