#include "responser12.h"

using namespace std;

/* 用户自己管理的应答器1 */
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
			case R1_FUN1:
				r1_fun1();
				break;
			case R1_FUN2:
				int a, b;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &a,
					sizeof(int), &b, -1);
				cout << "(1)The sum = " << r1_fun2(a, b) << endl;
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
			case R2_FUN1:
				double a, b;
				msgDataUnPack(eventMsg.msgData, sizeof(double), &a,
					sizeof(double), &b, -1);
				r2_fun1(a, b);
				break;
			case R2_FUN2:
				int c, d;
				msgDataUnPack(eventMsg.msgData, sizeof(int), &c,
					sizeof(int), &d, -1);
				cout << "(2)The sum is " << r2_fun2(c, d) << endl;
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

