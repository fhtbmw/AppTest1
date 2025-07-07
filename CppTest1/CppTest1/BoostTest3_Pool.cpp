#include "pch.h"
#include <stdint.h>
#include<iostream>
#include<boost/pool/pool.hpp>
#include<boost/pool/object_pool.hpp>
#include<boost/pool/pool_alloc.hpp>
#include<boost/pool/singleton_pool.hpp>

#define MG622AB_PACKAGE_MAX_SIZE 58

using namespace std;

typedef boost::pool<> pool2;



/*
Pool<>为普通内置类型分配空间
*/
void TestPool() {
	boost::pool<>pl1(sizeof(int));
	pool2 pl2(sizeof(char));
	int* p1 = (int*)pl1.malloc();	//malloc返回vodi* 指针，使用时要转换
//	assert(p1.is_from(pl1));		//判断p1的空间是否是pll内存池分配的
//	assert(pll::is_from(p1))

	pl1.free(p1);
	pl1.ordered_malloc(100);		//ordered_malloc可连续分配内存
}

/*
objecet_pool 为类对象分配内存
*/
struct Test {
	int a,b;
	char name;

	Test(int n=1,int m=2):a(n),b(m) {
		cout<<"Construct class Obj "<<this << endl;
	}
	~Test() {
		cout<<"Destroy Test Obj "<<this << endl;
	}
};

void TestObectPool() {
	boost::object_pool<Test>op1;
	//Test* t1 = op1.construct();
	//assert(t1!=NULL);
	boost::object_pool<string>op2;
	string* s;
	for (int i = 0; i < 20;i++) {
 		s=op2.construct("||||");
	}
	cout << *s;
}

/*

single_pool提供单例模式的内存池

*/

struct MyClass {};

typedef singleton_pool < MyClass, sizeof(MyClass) >sp1;

void TestSinglePool()
{
	MyClass* mc1 =(MyClass*) sp1::malloc();
}

//# pragma(push,2)
typedef struct T1
{

	uint8_t num1;
	uint16_t num2;
	union MyUnion
	{
		uint8_t arr[60];
	};
};
//# pragma(pop)
struct mg622ab_sync_payload_packet_t
{
	uint8_t index;
	uint8_t length;
	uint8_t data_size;
	uint8_t data[MG622AB_PACKAGE_MAX_SIZE];
	uint8_t checksum;
};

struct mg622ab_sync_payload_t
{
	uint8_t ctrl;
	uint8_t ctrl_aux;

	union
	{
		uint8_t data[64 - 2];
		mg622ab_sync_payload_packet_t packet;
	}payload;

};

void TestInt()
{
	//T1 t11;
	//assert(sizeof(t11)==3);
	mg622ab_sync_payload_packet_t mgpp1;
	mg622ab_sync_payload_t mg1;
	cout<<sizeof(mg1)<<endl;
}

void TestClass()
{
	class a {
	public:
		void funa() {};
	};

	class b :public a {};

	b bb;
	int a[3][5];
	cout << sizeof(a);
}

void main()
{
	//TestObectPool();
	TestClass();
}










