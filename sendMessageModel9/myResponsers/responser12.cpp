#include "responser12.h"
#include "listener12.h"

using namespace std;

/* 用户自己管理的应答器1 */
responser1::responser1() : responser("responser1")
{
	//pthread_create(&resThreadId, NULL, onResponser1Thread, (void*)this);
	//pthread_setname_np(resThreadId, "responser1");
}

responser1::responser1(const char* pthreadName) : 
	responser((pthreadName == NULL) ? "responser1" : pthreadName)
{
	
}

responser1::~responser1()
{
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
			case R1_FUN1:
			{
				r1_fun1();
				/* 发送消息给监听器，返回结果 */
				MSG_INFO msgInfo;
				msgInfo.resType = RES1;
				msgInfo.msg.eventID = L1_FUN1;
				
				#if USE_PROTO_BUF
				student st;
				baseInfo* pBaseInfo = st.mutable_base_info();
				pBaseInfo->set_name("fanchenxin");
				pBaseInfo->set_age(27);
				pBaseInfo->set_sex("man");

				st.set_student_id(20120101);

				pBaseInfo = st.add_parents();
				pBaseInfo->set_name("father");
				pBaseInfo->set_age(50);
				pBaseInfo->set_sex("man");

				pBaseInfo = st.add_parents();
				pBaseInfo->set_name("mother");
				pBaseInfo->set_age(48);
				pBaseInfo->set_sex("woman");

				int size = st.ByteSize();
				//cout << "size = " << size;
				msgInfo.msg.dataSize = size;
				memset(msgInfo.msg.msgData, 0, size);
				st.SerializeToArray(msgInfo.msg.msgData, size);  //序列化 方式1
		
				#else
				/* 必须用new 开辟内存是因为MY_INFO 结构体
				    里有string 成员，只有用new 才能调用其
				    构造函数。*/
				MY_INFO* myinfo = new MY_INFO(); 
				myinfo->name = "fanchenxin";
				myinfo->age = 27;
				myinfo->sex = 'm';
				msgDataPack(msgInfo.msg.msgData, sizeof(MY_INFO), myinfo, -1);
				delete myinfo;
				#endif
				
				sendMsgToListener(msgInfo);
				break;
			}
			case R1_FUN2:
			{
				int a, b;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &a,
					sizeof(int), &b, -1);
				int sum = r1_fun2(a, b);
				cout << "(1)The sum = " << sum << endl;

				/* 发送消息给监听器，返回结果 */
				MSG_INFO msgInfo;
				msgInfo.resType = RES1;
				msgInfo.msg.eventID = L1_FUN2;
				msgDataPack(msgInfo.msg.msgData, sizeof(int), &sum, -1);
				sendMsgToListener(msgInfo);
				break;
			}
			default:
				break;
		}
		
		pthread_mutex_unlock(&mutex);
	}
}

/*	void* onResponser1Thread(void* arg)
{
	responser1* pRes1 = (responser1*)arg;
	pRes1->onResponser();
}*/	

/* 应答器2 */
responser2::responser2() : responser("responser2")
{
	//pthread_create(&resThreadId, NULL, onResponser2Thread, (void*)this);
	//pthread_setname_np(resThreadId, "responser2");
}

responser2::responser2(const char* pthreadName) :
	responser((pthreadName == NULL) ? "responser2" : pthreadName)
{
}


responser2::~responser2()
{
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
			case R2_FUN1:
			{
				double a, b;
				msgDataUnPack(eventMsg.msgData, sizeof(double), &a,
					sizeof(double), &b, -1);
				double res = r2_fun1(a, b);

				/* 发送消息给监听器 */
				MSG_INFO msgInfo;
				msgInfo.resType = RES2;
				msgInfo.msg.eventID = L2_FUN1;
				msgDataPack(msgInfo.msg.msgData, sizeof(double), &res, -1);
				sendMsgToListener(msgInfo);
				break;
			}
			case R2_FUN2:
			{
				int c, d;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &c,
					sizeof(int), &d, -1);
				int sum = r2_fun2(c, d);
				cout << "(2)The sum is " << sum << endl;

				/* 发送消息给监听器 */
				MSG_INFO msgInfo;
				msgInfo.resType = RES2;
				msgInfo.msg.eventID = L2_FUN2;
				msgDataPack(msgInfo.msg.msgData, sizeof(int), &sum, -1);
				sendMsgToListener(msgInfo);
				break;
			}
			default:
				break;
		}
		
		pthread_mutex_unlock(&mutex);
	}
}

/*	void* onResponser2Thread(void* arg)
{
	responser2* pRes2 = (responser2*)arg;
	pRes2->onResponser();
}*/	

