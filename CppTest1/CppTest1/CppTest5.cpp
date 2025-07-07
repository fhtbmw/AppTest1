#include "pch.h"
#include<iostream>
#include<string>
#include<vector>
#include <memory>
#include <complex>
#include<cstdio>
#include <thread>
#include <windows.h>
#include <assert.h>
#include <chrono>
#include <cassert>
#include <cstdio>
#include <fstream>
#include <locale>
#include <stdexcept>
#include <mutex>
#include <ctime>

using namespace std;

template<typename T1,typename T2>
std::ostream& operator << (std::ostream& str,const std::pair<T1,T2>& p)
{
	return str<<"["<<p.first <<"," << p.second << "]";
}

template<typename T1,typename T2>
void f(std::pair<T1,T2>);

void g(std::pair<int,int>);

void test1()
{
	pair<int, int>p(2,3);
	//cout << p;
	//complex<int>c;
}

struct a {
	char a;
	int b;
};


//smart pointer

void test2()
{
	std::shared_ptr<string> pN(new string("dd"));
	std::shared_ptr<int>pI(new int(2));

	(*pN)[0] = 'a';
	pN->replace(0,1,"c");
	//std::remove();
}

class Person
{
public:
	string name;
	shared_ptr<Person>mom;
	shared_ptr<Person>dad;
	vector<shared_ptr<Person>>kids;

	Person(const string & n,shared_ptr<Person> m=nullptr,shared_ptr<Person> f=nullptr)
		:name(n),mom(m),dad(f)
	{

	}

	~Person()
	{
		cout<<"delete"<<name << endl;
	}
};

shared_ptr<Person> InitFamliy(const string&name)
{
	shared_ptr<Person> mom(new Person(name +"`s mom"));
	shared_ptr<Person> dad(new Person(name +"`s dad"));
	shared_ptr<Person> kid(new Person(name,mom,dad));
	mom->kids.push_back(kid);
	dad->kids.push_back(kid);
	return kid;
}

void test3()
{
	shared_ptr<Person>p1= InitFamliy("a");
	
	p1 = InitFamliy("b");
}

// shared_from_this 获取类对象的this指针，在异步操作中保持对象存活


class AsyncWorker :public std::enable_shared_from_this<AsyncWorker>
{
public:

	void dowork()
	{
		cout << "work" << endl;
	}

	void startAsync()
	{
		auto self = shared_from_this();
		std::thread([self]()
		{
			self->dowork();
		}).detach();
	}

	
};

void test4()
{
	auto work = std::make_shared<AsyncWorker>();
	work->startAsync();
	std::this_thread::sleep_for(std::chrono::seconds(1));
}

//template

template<typename tp>
class t2 {
public:

	typedef typename tp::t t;

	void test();
};

template<typename tp>
void t2<tp>::test()
{
	t t1;
}

const BYTE datas[] = {
   0x01
};


template<typename T1, typename T2>

using common_type = std::common_type<T1, T2>;		//std::common_type<>确定所有类型 T... 的共用类型，即所有 T... 都能隐式转换到的类型

template<typename ... Args>		//接收多个模板参数
void fun(Args... args)
{

}

template<typename T>
struct type_trait;

template<typename T1, typename T2>
struct type_trait<std::pair<T1, T2>>
{
	using FirstType = T1;
	using SecondType = T2;
};

template<typename T, typename U, typename ... Rest>
struct type_trait2 
{};

void TestTemp()
{
	cout<<std::is_pointer<int>::value<<endl;	// std::is_pointer<> 判断类型是否是指针类型
	cout<<std::is_pointer<int*>::value<<endl;
	
	BYTE cbNum1 = 0;
	WORD wStr1 = 's';
	fun(cbNum1);
	fun(cbNum1,wStr1);

	using TestPair = std::pair<BYTE, WORD>;
	using firsttype = type_trait<TestPair>::FirstType;
	using secondtype = type_trait<TestPair>::SecondType;

	is_const<int* const>::value;	//true	指针为常量指针，指针不可移动	
	is_const<const int*>::value;	//false	 指针指向的数为常量,指针可移动

}

void test5()
{
	std::unique_ptr<std::string> up1 (new string ("bedspreads"));
	(*up1)[0] = 'd';
	cout<<up1->capacity();
	
	std::string* sp1 = up1.release();	//unique ptr 释放所有权
	
	assert(up1.get() == nullptr);
	up1.reset();	// 等效 up1 = nullptr;

	std::string* strp2 = new string("Lyndsay's");
	std::unique_ptr<std::string> up2(strp2);
	std::unique_ptr<std::string> up3(std::move(up2));
	
	up2.swap(up1);

	std::unique_ptr<std::string[]>up4 (new string[2]);
	up4[0] = 'd';

	assert(std::is_pointer<std::unique_ptr<std::string>>::value == FALSE);

	TestTemp();
}

struct B
{
	virtual ~B() = default;

	virtual void bar() { std::cout << "B::bar \n"; }
};

struct D :B 
{
	D() { std::cout << "D" << endl; }

	~D() { std::cout << "~D" << endl; }

	void bar() override { std::cout << "D::bar \n"; }
};

std::unique_ptr<D> pass_through(std::unique_ptr<D> p)
{
	p->bar();
	return p;
}

class _spin_lock
{
public:
	_spin_lock() :flag(ATOMIC_FLAG_INIT) {}

	void lock()
	{
		while (flag.test_and_set(std::memory_order_acquire));
		{

		}
	}

	void unlock()
	{
		flag.clear(std::memory_order_release);
	}

private:
	std::atomic_flag flag;
};

_spin_lock splk;
int g_num = -1;

void func1()
{
	
	std::lock_guard<_spin_lock> lg(splk);
	g_num++;
	cout << g_num << endl;
}

void test5_1()
{
	/*_spin_lock lk1;
	lk1.lock();
	lk1.unlock();*/
}


//STL clock Timer

void ff()
{
	volatile double num = 0;
	for (int i = 0 ; i<1000; i++)
	{
		for (int j = 0; j < 1000; j++)
		{
			num += num* i * j;
		}
	}
}

void test6()
{
	std::clock_t c_s = std::clock();	//std::clock() 计算cpu运行时间
	
	auto t_s = std::chrono::high_resolution_clock::now();
	std::thread tt1(ff);
	std::thread tt2(ff);
	tt1.join();
	tt2.join();

	std::clock_t c_e = std::clock();

	auto t_e = std::chrono::high_resolution_clock::now();
	
	std::chrono::duration<double> duration = t_e - t_s;
	double Time = double(c_e - c_s) / CLOCKS_PER_SEC;


	cout << Time << "s" << endl;
	cout << duration.count() << "s" << endl;
}


void main()
{
	//test5();

	//TestTemp();
	test5();
	//CopyMemory(arr.data(),&device[0].uKey[0][0],sizeof(device[0].uKey[0]));
}













