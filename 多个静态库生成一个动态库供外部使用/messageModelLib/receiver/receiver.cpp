#include "receiver.h"
#include "responser.h"

using namespace std;

/* 消息接收器(消息转发中心) */
static receiver s_receiver_instance;
receiver *getReceiverInstance()
{
	return &s_receiver_instance;
}

void* onResThread(void * arg);

int receiver::responserMaxNum = RES_MAX;

receiver::receiver():responserNum(0), recThreadId(0), priortyMap(NULL)
{
	pthread_mutex_init(&mutex,NULL);
	pthread_create(&recThreadId, NULL, onResThread, (void*)this);
	pthread_setname_np(recThreadId, "receiver");
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
	if(resType >= responserMaxNum || !pRes)
		return;
	if(responserNum >= responserMaxNum)
		return;
	resMap.insert(make_pair(resType, pRes));
	priortyMap[resType] = res_prior;
	responserNum++;
	cout << "add responser success num = " << responserNum << endl;
}

void receiver::deleteResponser(RES_TYPE resType)
{
	RES_MAP::iterator ite;
	ite = resMap.find(resType);
	if(ite != resMap.end()){
		resMap.erase(ite);
		priortyMap[resType] = 0;
		cout << "delete Responser : " << resType << " success !!!" << endl;
	}else{
		cout << "in the recmap can not find responser: "<< resType << endl;
	}
}

void receiver::addMsgIntoResQue(MSG_INFO msgInfo)
{
	pthread_mutex_lock(&mutex);
	#if 0
	switch(priortyMap[msgInfo.resType])
	{
		case HIGH:
			resHighQueue.push(msgInfo);
			break;
		case MID:
			resMidQueue.push(msgInfo);
			break;
		case LOW:
			resLowQueue.push(msgInfo);
			break;
		default:
			resMidQueue.push(msgInfo);
			break;
	}
	#else
	RES_PRIOR priority = LOW;
	priority = priortyMap[msgInfo.resType];
	if(priority >= LOW && priority <= HIGH){
		resQueues[priority].push(msgInfo);
	}
	#endif
	pthread_mutex_unlock(&mutex);
}

void mutex_clean(void* mutex)
{
	pthread_mutex_unlock((pthread_mutex_t*)mutex);
}

void receiver::processResMsg()
{
	while(1){
		#if 0
		std::queue<MSG_INFO> *recQueue = NULL;
		while(1){
			//cout << "loop get message!!!!" << endl;
			if(!resHighQueue.empty()){
				recQueue = &resHighQueue;
				//cout << "receiver high queue have data!!!!" << endl;
				break;
			}
			else if(!resMidQueue.empty()){
				recQueue = &resMidQueue;
				//cout << "receiver mid queue have data!!!!" << endl;
				break;
			}
			else if(!resLowQueue.empty()){
				recQueue = &resLowQueue;
				//cout << "receiver low queue have data!!!!" << endl;
				break;
			}
		}
		#else
		RES_QUE *recQueue = NULL;
		int priority = HIGH; 
		for(; priority >= LOW; --priority){    //优先级从高到低
			if(!resQueues[priority].empty()){
				recQueue = &resQueues[priority];
				break;
			}
		}
		#endif
		
		if((recQueue != NULL) && !recQueue->empty()){
			int old_cancel;
			pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &old_cancel);
			pthread_cleanup_push(mutex_clean, &mutex);
			pthread_mutex_lock(&mutex);
			MSG_INFO pMsgInfo = recQueue->front();
			RES_MAP::iterator ite = resMap.begin();
			for(; ite != resMap.end(); ite++){
				if(ite->first == pMsgInfo.resType)
					break;
			}
			if(ite == resMap.end()) continue;

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

void* onResThread(void* arg)
{
	receiver* pRec = (receiver*)arg;
	pRec->processResMsg();
}
