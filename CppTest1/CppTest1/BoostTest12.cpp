#define BOOST_DATE_TIME_SOURCE
#define BOOST_THREAD_NO_LIB

#include "pch.h"
#include<string>
#include<stack>
#include<atomic>
#include<iostream>
#include<windows.h>
#include<boost/bind.hpp>
#include<boost/thread.hpp>

using namespace boost;
using namespace std;

/*

�߳�

*/

mutex mu_io;
mutex mu;
DWORD tid;

void func1(std::atomic<int>&x,const string& str)
{
	mutex::scoped_lock lock(mu);
	tid=GetCurrentThreadId();
	cout << tid << endl;
	try {
		for (int i = 0; i < 5; i++)
		{
			//this_thread::sleep(posix_time::milliseconds(1));
			mutex::scoped_lock lock(mu_io);
			cout << str << ++x << endl;
			this_thread::interruption_point();
		}
	}
	catch (thread_interrupted&)
	{
		cout<<"thread_interrupted" << endl;
	}
}



void TestMutex()
{
	std::atomic<int> x;
	std::atomic<int>y = 4;
	std::atomic<int>z = 4;
	
	boost::thread t1(func1,boost::ref(x),"t1 ");
	boost::thread t2(func1,boost::ref(x),"t2 ");

	//t1.join();	//�����ȴ�
	t1.timed_join(posix_time::microseconds(0));	//��ʱ�ȴ�
	t2.join();
	//this_thread::sleep(posix_time::seconds(2));

	boost::function<void()>f = boost::bind(func1,boost::ref(y),"t3 ");
	thread t3(f);
	t3.interrupt();
	thread t4(boost::bind(func1,boost::ref(z),"t4 "));

	t3.join();
	t4.join();
	cout<<t4.hardware_concurrency()<<endl;
}

void func2(std::atomic<int>&x,const string& s)
{
	try 
	{
		using namespace this_thread;
		assert(interruption_enabled());
		for (int i=0;i<5;i++)
		{
			disable_interruption di;		//�ر��ж�
			assert(!interruption_enabled());
			mutex::scoped_lock lock(mu_io);
			
			cout << s << ++x << endl;
			cout<<"Ҫ���ж�:" << interruption_requested() << endl;
			interruption_point();
			restore_interruption ri(di);	//�ָ��ж�
			assert(interruption_enabled());
			cout << "�����ж�:" << interruption_enabled() << endl;
			interruption_point();
		}

	}
	catch (thread_interrupted&)
	{
		cout<<"thread interrupted" << endl;
	}
}

void TestMutex2()
{
	std::atomic<int>x;
	thread t1(boost::bind(func2,boost::ref(x),"t1 "));
	thread t2(boost::bind(func2,boost::ref(x),"t2 "));

	t1.interrupt();
	t1.join();
	t2.join();
}

/*

�߳���
thread_group tg;
tg.create_thread();
tg.joinall()

*/

/*

��������,�ͻ��������ʹ��


*/

//��������ģ��


mutex io_mu;
mutex mu1;

class Buffer 
{
public:
	Buffer(size_t n):to_read(0),capacity(n) {}
	void put(int x);
	void get(int* x);
private:
	stack<int> b_s;
	int to_read,capacity;
	condition_variable_any con_put;
	condition_variable_any con_get;
	bool isEmpty()
	{
		return to_read == 0;
	}
	bool isFull()
	{
		return to_read == capacity;
	}
};

void Buffer::put(int x)
{
	
		
	<mutex> lock(mu1);;
		while (isFull())
		{
			{
				lock_guard<mutex> io_lock(io_mu);
				cout << "Produce Thread: " << GetCurrentThreadId() << " waitting " << endl;
			}
			con_put.wait(lock);
		}
		b_s.push(x);
		++to_read;
		lock.unlock();
	
	con_get.notify_one();	//֪ͨ��Ӧ����������
}

void Buffer::get(int *x)
{
	
		unique_lock<mutex> lock(mu1);
		while (isEmpty())
		{
			{
				lock_guard<mutex> io_lock(io_mu);
				cout << "Consume Thread: " << GetCurrentThreadId() << " waitting" << endl;
			}
			con_get.wait(lock);
		}
		*x = b_s.top();
		b_s.pop();
		--to_read;
		lock.unlock();
	con_put.notify_one();	//֪ͨ��Ӧ����������
}

Buffer Bu(5);

void producer(int n)
{
	for (int i=0;i<n;i++)
	{
		{
			lock_guard<mutex> io_lock(io_mu);
			cout << "put" << i << endl;
		}
		Bu.put(i);
	}
}

void consumer(int n)
{
	int x;
	for (int i=0;i<n;i++)
	{
		Bu.get(&x);
		{
			lock_guard<mutex> io_lock(io_mu);
			cout << "get:" << x << endl;
		}
	}
}

void TestCondition()
{
	/*thread_group tg;
	tg.create_thread(bind(producer,20));
	tg.create_thread(bind(consumer,10));
	tg.create_thread(bind(consumer,10));
	tg.join_all();*/
	thread t1(boost::bind(producer,20));
	thread t2(boost::bind(consumer,10));
	thread t3(boost::bind(consumer,10));
	t1.join();
	t2.join();
	t3.join();
}

/*

��������shred_mutex,�̻߳�ȡ�������Ȩ��һ��ר��Ȩ

*/
class rw_data
{
public:
	rw_data() :m_x(0) {}
	void write(int x);
	void read(int *x);
private:
	int m_x;
	shared_mutex rw_mu;
};

void rw_data::write(int x)
{
	unique_lock<shared_mutex> lock(rw_mu);
	m_x++;
}

void rw_data::read(int *x)
{
	shared_lock<shared_mutex> lock(rw_mu);
	*x = m_x;
}


/*

future �첽����

*/

int fab(int n)
{
	if (n==0||n==1)
	{
		return 1;
	}
	else
	{
		return fab(n - 1) + fab(n-2);
	}
}

void TestFuture()
{
	packaged_task<int> pt(boost::bind(fab,10));		//���㣬packaged-task�����޲κ���
													//ͨ��bind���뺯������		
	unique_future<int> uf = pt.get_future();		//unique_future ���ܼ����futureֵ
	thread t1(boost::move(pt));						//packaged_task���󲻿ɿ�����ͨ��moveת��
	uf.wait();	
	cout << uf.get();
}

/*

�̱߳��ش洢,����ʹ���˾�̬�������ڶ��̻߳����л�����쳣
thread_specific_ptrʹ���߳����Լ������ݿ���

*/

//�߳̽���ʱ�ص�����
void Call_end(const string& s)
{
	cout << this_thread::get_id() << " Thread exit" << s << endl;
}


void print()
{
	thread_specific_ptr<int> pi;
	pi.reset(new int());	//reset()��ֵ

	++ (*pi);
	this_thread::at_thread_exit(boost::bind(Call_end," bye"));	//�߳̽������ú���
	mutex::scoped_lock lock(io_mu);
	cout<<this_thread::get_id()<<" thread value: "<<*pi << endl;
}

void TestThreadSpecific()
{
	thread_group tg;
	tg.create_thread(print);
	tg.create_thread(print);

	tg.join_all();
}


void main()
{
	TestMutex();
	//TestThreadSpecific();
}
