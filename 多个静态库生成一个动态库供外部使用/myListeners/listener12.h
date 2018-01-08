#include "myComDef.h"
#include "sendMsgModel.h"

/* 用户自己管理的监听器1 */
void* onListener1Thread(void * arg);

class listener1: public listener
{
public:
	listener1();
	~listener1();
	/* fun1 和fun2 为各自应答器需要执行的函数 */
	void l1_fun1();
	void l1_fun2(int sum);
	virtual void onListener();
private:
	pthread_t lisThreadId;
};


/* 监听器2 */
void* onListener2Thread(void * arg);

class listener2: public listener
{
public:
	listener2();
	~listener2();
	void l2_fun1(double res);
	void l2_fun2(int sum);
	virtual void onListener();
private:
	pthread_t lisThreadId;
};

typedef enum listen_event_type
{
    L1_FUN1 = 0,
    L1_FUN2,
    L2_FUN1,
    L2_FUN2,
    LX_NULL
}LIS_EVEN_TYPE;
