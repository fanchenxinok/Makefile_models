#include "sendMsgModel.h"
#include "myComDef.h"

/* 用户自己管理的应答器1 */
void* onResponser1Thread(void * arg);

class responser1: public responser
{
public:
	responser1();
       responser1(const char* pthreadName);
	~responser1();
	/* fun1 和fun2 为各自应答器需要执行的函数 */
	void r1_fun1();
	int r1_fun2(int a, int b);
	virtual void onResponser();
};


/* 应答器2 */
void* onResponser2Thread(void * arg);

class responser2: public responser
{
public:
	responser2();
       responser2(const char* pthreadName);
	~responser2();
	double r2_fun1(double a, double b);
	int r2_fun2(int a, int b);
	virtual void onResponser();
};

typedef enum
{
    RES1 = 0,
    RES2,
    RES3,
    RES4,
    MAX_RES
};

typedef enum event_type
{
    R1_FUN1 = 0,
    R1_FUN2,
    R2_FUN1,
    R2_FUN2,
    RX_NULL
}EVEN_TYPE;
