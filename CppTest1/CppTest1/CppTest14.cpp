#include "pch.h"

#include <string>
#include <utility>
#include <functional>
#include <ctime>
#include <execution>
#include <future>
#include <thread>
#include <chrono>
#include <random>
#include <exception>
#include <regex>
#include <vector>
#include <list>
#include <stack>
#include <string>
#include <iostream>
#include <condition_variable>
#include <future>
#include <queue>
#include <ctype.h>
#include <numeric>
#include <stdexcept>
#include <mutex>
#include <shared_mutex>
#include <windows.h>

#define PA (P) (P)

using namespace std;

//regex 正则表达式库

void _Out(bool flag)
{
	cout << (flag ? "exist" : "not exist") << endl;
}

void test1()
{
	regex re1("(.*)");		// . -> 任何字符 * -> 出现任何次数 

	bool ret = regex_match("dasad><", re1);

	_Out(ret);

	bool ret2 = regex_match("dsad>", regex(".* \\1"));

	_Out(ret2);

}

void test_regex2()
{
	bool ret;

	ret = regex_match("<rag>value</tag>", regex("<(\d)>.*<?>"));	// \d -> 匹配数字

	ret = regex_match("asa0_D", regex("\w+\d+"));		// + -> 匹配前面字符一次或多次 ? -> 匹配0次或多次	

	std::smatch _match;		//匹配string 
	std::cmatch _cmatch;	//匹配c_str

	string str = "adasd121`3";

	ret = regex_search(str, _match, regex(R"(\d?)"));

	_Out(ret);
}

void test_regex3()
{
	std::string str = "sdasad3242_";

	std::string replace = "000000";

	string ret = std::regex_replace(str, regex(R"(\d+)"), replace);	// R() 原始字符字面量，里面的内容无需再转义 "\" 

	cout << ret << endl;

	std::string str2 = "asdadsa<>sad<>";

	bool ret2 = regex_match(str2, regex("<.*>.*<.*>"));

	_Out(ret2);
}




/*
	sync
*/

int func(char c)
{
	std::default_random_engine dre(c);
	std::uniform_int_distribution<int> uid(1, 100);

	for (int i = 0; i < 10; i++)
	{
		this_thread::sleep_for(chrono::milliseconds(uid(dre)));
		cout.put(c).flush();
	}
	return c;
}

int func1()
{
	return func('1');
}

int func2()
{
	return func('2');
}

void test2()
{
	std::future<int> res(std::async(func1));	//std::future 类模板，获取异步操作的结果		

	int res2 = func2();

	int ret = res2 + res.get();

	cout << "result: \n" << ret << endl;
}

void task1()
{
	list<int>v;
	for (int i = 0; i < 100; i++)
	{
		v.push_back(i);
	}
	cout.put('.').flush();
}

void test3()
{
	auto f1 = async(task1);	//start task1

	f1.wait_for(std::chrono::seconds(2));

	cin.get();

	try
	{
		f1.get();
	}
	catch (const std::exception& e)
	{
		cerr << "EXCEPTION" << e.what() << endl;
	}


}


void test4()
{
	std::future<int>fu1 = std::async(std::launch::async, bind(func, '1'));		//异步任务立刻执行

	int ret1 = fu1.get();

	cout << "fu1 : " << ret1 << endl;

	std::future<int>fu2 = std::async(std::launch::deferred, bind(func, '2'));	//defered 任务执行策略

	int ret2 = fu2.get();														//调用get时才执行任务

	cout << "fu2 : " << ret2 << endl;;

	std::future<int>fu3 = std::async(bind(func, '3'));		//默认执行策略

	int ret3 = fu3.get();

	cout << "fu3 : " << ret3 << endl;
}

void test5()
{
	list<int> l1;

	std::future<int> fu1 = std::async(bind(func, '0'));

	fu1.wait();			//等待执行结束阻塞当前线程等待

	fu1.wait_for(std::chrono::seconds(2));			//超时等待

	fu1.wait_until(std::chrono::system_clock::now() + std::chrono::minutes(1));
}

void _do(char c)
{
	default_random_engine dre(c);
	uniform_int_distribution<int> id(10, 1000);

	for (int i = 0; i < 10; i++)
	{
		auto tid = this_thread::get_id();
		this_thread::sleep_for(chrono::milliseconds(id(dre)));

		cout.put(c).flush();
	}
}

void test6()
{
	auto f1 = async(std::bind(_do, '-'));
	auto f2 = async(bind(_do, '+'));

	if (f1.wait_for(chrono::seconds(0)) != future_status::ready ||
		f2.wait_for(chrono::seconds(0)) != future_status::ready)
	{
		this_thread::yield();
	}
	cout.put('\n').flush();

	try
	{
		f1.get();
		f2.get();
	}
	catch (const exception& e)
	{
		cout << "EXCEPTION " << e.what() << endl;
	}
}

int queryNumber()
{
	cout << "read num" << endl;

	int num;

	cin >> num;

	if (!cin)
	{
		throw runtime_error("no input");

	}
	return num;
}


void _do2(char c, shared_future<int> f)
{
	try
	{
		int num = f.get();	//获取另一个异步线程的处理结果

		for (int i = 0; i < num; i++)
		{
			this_thread::sleep_for(chrono::seconds(1));
			cout.put(c).flush();
		}

	}
	catch (const exception& e)
	{
		cerr << "EXCEWPTION in thread" << this_thread::get_id()
			<< ":" << e.what() << endl;
	}

}

void test7()
{
	//shared future

	try
	{
		shared_future<int>shf = async(queryNumber);		//多个线程可共享shared_future的结果

		future<void> f1 = async(launch::async, bind(_do2, '[', shf));
		future<void> f2 = async(launch::async, bind(_do2, ']', shf));
		future<void> f3 = async(launch::async, bind(_do2, '(', shf));

		f1.get();
		f2.get();
		f3.get();

	}
	catch (const std::exception& e)
	{
		cout << "exception " << e.what() << endl;
	}
	cout << "done" << endl;
}


void _do3(int num, char c)
{
	try
	{
		default_random_engine dre(42 * c);
		uniform_int_distribution<int> id(10, 1000);

		for (int i = 0; i < num; i++)
		{
			this_thread::sleep_for(chrono::milliseconds(id(dre)));
			cout.put(c).flush();
		}
	}
	catch (const exception& e)
	{
		cerr << " THREAD_EXCEPTION thread: " << this_thread::get_id() << e.what() << endl;
	}
}

void test8()
{
	//thread 
	thread::id mainthread;

	if (this_thread::get_id() == mainthread)
	{
		cout << "==" << endl;
	}

	try
	{
		thread t1(bind(_do3, 3, '.'));

		for (int i = 0; i < 10; i++)
		{
			thread t(bind(_do3, 3, 'a' + i));

			cout << "detach thread: " << t.get_id() << endl;
			t.detach();
		}

		cin.get();
		cout << "join thread: " << t1.get_id() << endl;
		t1.join();
	}
	catch (const exception& e)
	{
		cerr << "EXCEPTION" << e.what() << endl;
	}
}

void Thread_func1(std::promise<int>& p)
{
	std::this_thread::sleep_for(std::chrono::seconds(1));

	int value = 6;
	p.set_value(value);
}

void Thread_func2(std::future<int>& f)
{
	auto value = f.get();
	cout << "get value " << value << endl;
}

void test9()
{
	//promise
	std::promise<int> pr1;			//promise 设置数据，通过future对象获取,在线程间传数据

	std::future<int> fu1 = pr1.get_future();	//获取数据

	auto ret = pr1.get_future().share();	//返回shared_future

	std::thread t1(Thread_func1, std::ref(pr1));	//t1线程设置的数据t2线程可以获取
	std::thread t2(Thread_func2, std::ref(fu1));

	t1.join();
	t2.join();
}

void _do4(std::promise<std::string>& p)
{
	try
	{
		cout << "read char " << endl;

		char c = cin.get();
		if (c == 'e')
		{
			throw std::runtime_error(std::string("char") + c);
		}

		std::string s = std::string("char") + c;
		p.set_value(s);
	}
	catch (...)
	{
		p.set_exception(std::current_exception());
	}
}

void test10()
{
	try
	{
		std::promise<string> p1;
		std::future<string> fu1(p1.get_future());

		thread t1(bind(_do4, std::ref(p1)));
		t1.detach();

		cout << "get : " << fu1.get() << endl;
	}
	catch (const std::exception& e)
	{
		cerr << "EXCEPTION" << e.what() << endl;
	}
}

void _do5(std::future<std::string>& s)
{
	try
	{
		cout << "get value :" << s.get() << endl;
	}
	catch (const exception& e)
	{
		cerr << "EXCEPTION" << e.what() << endl;
	}
}

void test11()
{
	try
	{
		std::promise<std::string> pr1;
		std::future<std::string> fu1(pr1.get_future());

		std::string s1("c");

		pr1.set_value(s1);

		thread t1(bind(_do5, std::ref(fu1)));
		t1.join();

	}
	catch (const exception& e)
	{
		cerr << "EXCEPTION" << e.what() << endl;
	}
}

void _do6(std::promise<int>& pr)
{
	try
	{
		int num = 0;
		//pr.set_value_at_thread_exit(num)
		pr.set_value(num);

		cout << "this_thread::get_id" << this_thread::get_id() << endl;
		cout << "thread::id :" << thread::id() << endl;

	}
	catch (const exception& e)
	{
		cerr << "EXCEPTion " << e.what() << endl;
	}
}

void test12()
{
	try
	{
		std::promise<int> pr1;
		std::future<int> fu1(pr1.get_future());

		thread t1(bind(_do6, std::ref(pr1)));
		t1.join();							//线程创建后需join / detach 

		auto tid = t1.get_id();

		cout << fu1.get() << endl;
		cout << "tid: " << tid << endl;

	}
	catch (const exception& e)
	{
		cerr << "EXCEPTION" << e.what() << endl;
	}
}


// Mutex Lock

#include <mutex>

std::mutex _mutex;

void _do7(const std::string& s)
{
	{
		std::lock_guard<std::mutex> l(_mutex);
		for (char c : s)
		{
			std::cout.put(c);
		}
		cout << endl;
	}
}

class DeadLockTest
{
public:
	void Func1()
	{
		std::lock_guard<mutex> lg(_mutex);
	}
	void Func2();

private:
	std::mutex _mutex;
};

void DeadLockTest::Func2()
{
	std::lock_guard<mutex> lg(_mutex);	//发生死锁，锁住_mutex，Func1再次锁_muex,通过递归锁解决
	Func1();
}

class LockTest
{
public:
	void Func1()
	{
		std::lock_guard<recursive_mutex> lg(_rmutex);
	}

	void Func2()
	{
		std::lock_guard<recursive_mutex> lg(_rmutex);

		Func1();
	}

private:
	std::recursive_mutex	_rmutex;
};

class TryLock
{
public:
	void Func1()
	{
		if (_mutex.try_lock() == true)	//Func2中锁住了_mutex,且调用Func1时未释放，try_lock获取锁失败
		{
			std::lock_guard<mutex> lg(_mutex, std::adopt_lock);	 //指明互斥量已在外部被锁住，不再锁该互斥量
			cout << "Func1 " << this_thread::get_id() << endl;
		}
		else
		{
			cout << "Func1 try_lock error" << endl;
		}
	}

	void Func2()
	{
		std::lock_guard<mutex> lg(_mutex);
		cout << "Func2" << this_thread::get_id() << endl;

		Func1();
	}

private:
	std::mutex	_mutex;
};

void test13()
{
	/*auto f1 = async(launch::async, bind(_do7, "thread1"));
	auto f2 = async(launch::async, bind(_do7, "thread2"));*/

	/*DeadLockTest dlt1;
	dlt1.Func2();*/

	/*LockTest lt1;
	lt1.Func2();*/

	TryLock tl1;
	tl1.Func2();

}

void shared_resource_access()
{
	std::mutex _mtx;

	_mtx.lock();

	std::unique_lock<std::mutex> lock(_mtx, std::adopt_lock);

	std::cout << "thread " << std::this_thread::get_id() << endl;
}

void test14()
{
	std::thread t1(shared_resource_access);
	std::thread t2(shared_resource_access);

	t1.join();
	t2.join();
}

void test15()
{
	std::mutex m1;
	std::mutex m2;

	int idx = std::try_lock(m1, m2);
	if (idx < 0)				//std::try_lock 锁住所有互斥量,成功后返回-1
	{
		std::lock_guard<std::mutex> lg1(m1, std::adopt_lock);
		std::lock_guard<std::mutex> lg2(m2, std::adopt_lock);
	}
	else
	{
		std::cerr << "could not lock mutex" << std::endl;
	}
}

// unique_lock

bool readyflag;
std::mutex readyFlagMutex;

void unique_thread1()
{
	std::lock_guard<std::mutex> lg(readyFlagMutex);
	readyflag = true;
}

void unique_thread2()
{
	{
		std::unique_lock<std::mutex> ul(readyFlagMutex);
		while (!readyflag)
		{
			ul.unlock();
			std::this_thread::yield();
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			ul.lock();
		}
	}
}

bool readyFlag;
std::mutex readyMutex;
std::condition_variable	readycv;

void thread1()
{
	std::cout << "<return>" << endl;
	std::cin.get();

	{
		std::lock_guard<std::mutex> lg(readyMutex);
		readyFlag = true;
	}

	readycv.notify_one();
}

void thread2()
{
	{
		std::unique_lock<std::mutex> ul(readyMutex);	//使用unique_lock非lock_guard
		readycv.wait(ul, [] { return readyFlag; });		//wait内部会对mutex解锁
	}

	cout << "done" << endl;
}

void test16()
{
	auto f1 = std::async(std::launch::async, thread1);
	auto f2 = std::async(std::launch::async, thread2);
}

std::queue<int> _queue;
std::mutex _queueMutex;
std::condition_variable queuecv;

void provider(int val)
{
	for (int i = 0; i < 6; i++)
	{
		std::lock_guard<std::mutex> lg(_queueMutex);
		_queue.push(val + 1);
	}
	queuecv.notify_one();

	std::this_thread::sleep_for(std::chrono::milliseconds(val));
}

void consumer(int num)
{
	while (true)
	{
		int val;
		{
			std::unique_lock<std::mutex> ul(_queueMutex);
			queuecv.wait(ul, [] { return !_queue.empty(); });
			val = _queue.front();

			_queue.pop();
		}
		std::cout << "consumer" << num << ":" << val << std::endl;
	}
}

void test17()
{
	auto p1 = std::async(std::launch::async, provider, 100);
	auto p2 = std::async(std::launch::async, provider, 200);
	auto p3 = std::async(std::launch::async, provider, 300);

	auto c1 = std::async(std::launch::async, consumer, 1);
	auto c2 = std::async(std::launch::async, consumer, 2);
}

std::mutex _mutex6;

class joining_thread
{
	std::thread _t;

public:
	joining_thread()noexcept = default;

	template<typename Callable, typename... Args>
	explicit joining_thread(Callable&& func, Args&&... arg) :
		_t(std::forward<Callable>(func), std::forward<Args>(arg)...) {}

	explicit joining_thread(std::thread _t)noexcept : _t{ std::move(_t) } {}

	joining_thread(joining_thread&& other)noexcept : _t{ std::move(other._t) } {}

	joining_thread& operator= (std::thread&& other)noexcept
	{
		if (joinable())
		{
			join();
		}
		_t = std::move(other);
	}

	~joining_thread()
	{
		if (joinable())
		{
			join();
		}
	}

	bool joinable()const noexcept
	{
		return _t.joinable();
	}

	void join()
	{
		_t.join();
	}

	void detach()
	{
		_t.detach();
	}

	void swap(joining_thread& other)noexcept
	{
		_t.swap(other._t);
	}

	std::thread::id get_id()const noexcept
	{
		return _t.get_id();
	}

	std::thread& data()noexcept
	{
		return _t;
	}

	const std::thread& data()const noexcept
	{
		return _t;
	}

};

void _do8(std::size_t i)
{
	//std::lock_guard<std::mutex> lk(_mutex6);
	_mutex6.lock();
	{
		cout << "id: " << i << endl;
	}
	_mutex6.unlock();
}

void test18()
{
	joining_thread thread;

	std::vector<joining_thread> vtthreads;
	std::vector<std::thread> vtthreads2;

	for (std::size_t i = 0; i < 10; i++)
	{
		//std::lock_guard<std::mutex> lg(_mutex6);
		vtthreads2.emplace_back(_do8, i);
	}

	for (auto& t :vtthreads2)
	{
		t.join();
	}

	std::thread t(bind(_do8, 10));
	std::thread t2(bind(_do8, 11));
	std::thread t3(bind(_do8, 12));
	t.join();
	t2.join();
	t3.join();

}

std::mutex _mutex7;

void add_to_list(int n, std::list<int>& l)
{
	std::vector<int> vt1(n + 1);
	std::iota(vt1.begin(), vt1.end(), 0);
	int sum = std::accumulate(vt1.begin(), vt1.end(), 0);

	{
		std::lock_guard<std::mutex> lg(_mutex7);
		l.push_back(sum);

 	}
}

void print_list(std::list<int>& l)
{
	//std::lock_guard<std::mutex> lg(_mutex7);
	//std::lock_guard lg{_mutex7};	//C++ 17新加标准，根据传递的锁判断其类型
	std::for_each(l.begin(), l.end(), [&](const int& value) 
		{
			cout << value << endl;
		});
}

void test19()
{
	std::list<int> l;

	std::thread t1(std::bind(add_to_list, 2, std::ref(l)));
	std::thread t2(std::bind(add_to_list, 3, std::ref(l)));
	std::thread t3(std::bind(print_list, std::ref(l)));
	std::thread t4(std::bind(print_list, std::ref(l)));
	std::thread t5(std::bind(print_list, std::ref(l)));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
}

std::mutex _mutex8, _mutex9;

void _do9()
{
	std::lock_guard<std::mutex> lg(_mutex8);
	std::lock_guard<std::mutex> lg2(_mutex9);
}

void _do10()
{
	std::lock_guard<std::mutex> lg(_mutex8);
	std::lock_guard<std::mutex> lg2(_mutex9);
}

void test20()
{
	std::thread t1(std::bind(_do9));
	std::thread t2(std::bind(_do10));

	t1.join();
	t2.join();
}

int g_val1 = 0;
std::shared_mutex _shmu;

void _write(int val)
{
	std::shared_lock<std::shared_mutex> lg(_shmu);
	g_val1 = val;
	cout << " write data:" << val << endl;
	//cout << ends;
}

void _read()
{
	std::shared_lock<std::shared_mutex> lg(_shmu);
	int data = g_val1;
	cout << " read data: " << data << endl;
}

void test21()
{
	int num = 2;
	int num2 = 3;

	std::thread t1(std::bind(_write, num));
	std::thread t2(std::bind(_write, num2));
	std::thread t3(std::bind(_read));
	std::thread t4(std::bind(_read));

	t1.join();
	t2.join();
	t3.join();
	t4.join();

	
}

thread_local int g_val2;	//绑定线程生命周期的变量
std::mutex _mutex10;

void func3()
{
	/*g_val2++;
	cout << g_val2<<endl;*/
	std::lock_guard<std::mutex> lg(_mutex10);
	g_val2++;
	cout << g_val2 << endl;
}

void test22()
{
	std::vector<std::thread> vtthtrads1;
	for (int i = 0 ; i < 20; i++)
	{
		/*std::thread t(func3);
		vtthtrads1.emplace_back(std::move(t));*/
		vtthtrads1.emplace_back(func3);
	}

	/*std::for_each(vtthtrads1.begin(), vtthtrads1.end(), [](std::thread t)
		{
			t.join();
		});*/
	for (auto & t :vtthtrads1)
	{
		if (t.joinable())
		{
			t.join();
		}
	}

	bool f1 = true;
	std::cout << std::thread::hardware_concurrency() << std::endl;
	std::cout << std::boolalpha;
	std::cout << f1;
}



template<typename T>
struct sorter
{
	struct chunk_to_sort
	{
		std::list<T> data;
		std::promise<std::list<T>> promise;
	};

	std::stack<chunk_to_sort> chunks;
	std::vector<std::thread> threads;
	unsigned const max_thread_count;
	std::atomic<bool> end_of_data;

	sorter(): max_thread_count(std::thread::hardware_concurrency() - 1),
			end_of_data(false)
	{}

	~sorter()
	{
		end_of_data = true;

		for (unsigned i = 0; i < threads.size(); ++i)
		{
			threads[i].join();
		}
	}

};

class father
{
};

class son :public father
{
	son();
	void son_fun();
};

//son::son(): father
//{
//
//}

void test23()
{
	//RegisterSuspendResumeNotification();
}

void main()
{
	//test22();
}







