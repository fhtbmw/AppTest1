#include <mutex>
#include <thread>
#include <functional>
#include <vector>
#include <algorithm>
#include <numeric>
#include <stdexcept>
#include <climits>
#include <iostream>

using namespace std;

//class some_big_object {};
//
//void swap(some_big_object& lhs, some_big_object& rhs) {}
//
//class X
//{
//public:
//	X(some_big_object const& sd) :some_detail(sd) {}
//
//	friend void swap(X& lhs, X& rhs)
//	{
//		if (&lhs == &rhs)
//		{
//			return;
//		}
//		std::unique_lock<std::mutex> lock_a(lhs._mutex, std::defer_lock);
//		std::unique_lock<std::mutex> lock_b(rhs._mutex, std::defer_lock);
//
//		std::lock(lock_a, lock_b);
//		swap(lhs.some_detail, rhs.some_detail);
//	}
//
//private:
//	some_big_object some_detail;
//	std::mutex _mutex;
//};
//
//std::unique_lock<std::mutex> get_lock()
//{
//	extern std::mutex _mutex;
//	std::unique_lock<std::mutex> lk(_mutex);
//	return lk;
//}	
//
//void process_data()
//{
//	std::unique_lock<std::mutex> lk(get_lock());
//	// ……
//}

void f1(int i, std::string& s) {};

void oops(int param)
{
	char buff[1024];
	sprintf_s(buff, "%i", param);

	thread t(bind(f1, param, std::string(buff)));
	t.detach();
}

void test1()
{
	oops(2);
}

std::mutex some_mutex;

void prepare_data()
{
	cout << "prepare_data " << endl;
}

void do_somethig()
{
	cout << "do_something" << endl;
}
//
//std::unique_lock<std::mutex>get_lock()
//{
//	std::unique_lock<std::mutex> lk(some_mutex);
//	prepare_data2();
//	return lk;
//}
//
//void prepare_data2()
//{
//	std::unique_lock<std::mutex> lk(get_lock());
//	do_somethig();
//}
//

void test2()
{
	{
		std::unique_lock<std::mutex> lock2(some_mutex, std::defer_lock);

		lock2.lock();		//unique_lock类型的锁可以被手动的加锁解锁
		do_somethig();
		lock2.unlock();
	}

	{
		//	std::unique_lock<std::mutex> lk(get_lock());
		do_somethig();
	}	//出作用域后，自动调用unlock
}

void delay_lock()
{
	bool flag;
	std::mutex mu;
	std::unique_lock<std::mutex> lk(mu, std::defer_lock);	//unique_lock 对象构建时不立即锁互斥量，需手动锁定

	lk.lock();
	if (lk.owns_lock())
	{
		lk.unlock();
	}

	{
		//if (lk.try_lock_for(std::chrono::milliseconds(1000)))	//延时锁定
		//{
		//	//……
		//}
	}
}

std::mutex _mu;
std::condition_variable _cv;
bool ready = false;

void worker()
{
	std::unique_lock<std::mutex> lk(_mu);
	_cv.wait(lk, []()
		{
			return ready;
		});
	cout << "worker" << endl;
}

void prepare()
{
	std::this_thread::sleep_for(chrono::seconds(1));

	std::lock_guard<std::mutex> lk(_mu);
	ready = true;
	_cv.notify_all();
}

void test2_1()
{
	std::thread t1(std::bind(worker));
	std::thread t2(std::bind(worker));
	std::thread t3(std::bind(worker));

	std::thread t4(std::bind(prepare));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
}

void test3()
{
	std::vector<std::thread> _threads;
	for (unsigned i = 0; i < 20; ++i)
	{
		_threads.push_back(std::thread(do_somethig));
	}
	std::for_each(_threads.begin(), _threads.end(),
		std::mem_fn(&std::thread::join));			//mem_fn 函数模板,生成指向指向成员函数的指针
}

template<typename IT, typename T>
struct acc
{
	void operator() (IT fir, IT la, T& res)
	{
		res = std::distance(fir, la);
	}

};

template<typename IT, typename T>
T para_acc(IT fir, IT la, T init)
{
	unsigned long const length = std::distance(fir, la);

	if (!length)
	{
		return init;
	}

	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency();
	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);
	unsigned long const block_size = length / num_threads;

	std::vector<T> result(num_threads);
	std::vector<std::thread> threads(num_threads - 1);

	IT block_start = fir;
	for (unsigned long i = 0; i < (num_threads)-1; ++i)
	{
		IT block_end = block_start;
		std::advance(block_end, block_size);	//移动迭代器位置
		threads[i] = std::thread(acc<IT, T>(), block_start, block_end, std::ref(result[i]));

		block_start = block_end;
	}

	acc<IT, T>()(
		block_start, la, result[num_threads - 1]);
	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));	//对threads中的每个线程进行join

	return std::accumulate(result.begin(), result.end(), init);
}

class some_data
{
public:
	void do_something()
	{
		cout << "some_data::do_something()" << endl;
	}

private:
	int a;
	std::string s;
};

class data_wrapper
{
public:
	template<typename F>
	void process_data(F f)
	{
		std::lock_guard<std::mutex> l(_mutex);
		f(_data);
	}

private:
	some_data _data;
	std::mutex _mutex;
};

some_data* somedata;

void malicious_func(some_data& _data)
{
	somedata = &_data;
}

void test4()
{
	data_wrapper x;
	x.process_data(malicious_func);
	somedata->do_something();


}

class some_big_obj {};

void swap2(some_big_obj& lhs, some_big_obj& rhs) {};

class T2
{
public:
	T2(some_big_obj obj) :_some_obj(obj) {}

	friend void swap(T2& rhs, T2& lhs)
	{
		if (&rhs == &lhs)
		{
			return;
		}

		std::lock(lhs._mutex, rhs._mutex);
		std::lock_guard<std::mutex> lg1(lhs._mutex, std::adopt_lock);
		std::lock_guard<std::mutex> lg2(lhs._mutex, std::adopt_lock);
		swap2(lhs._some_obj, rhs._some_obj);
	}

private:
	some_big_obj _some_obj;
	std::mutex _mutex;
};

//层次锁
class hierarchy_mutex
{
public:
	explicit hierarchy_mutex(unsigned long value) :hierarchy_value(value), pre_hierarchy_value(0) {}

	void lock()
	{
		check_value();
		inner_mutex.lock();
		update_value();
	}

	void unlock()
	{
		thread_local_hierarchy_value = pre_hierarchy_value; // 解锁时，将当前层级设置为上一次的层级，层级增加
		cout << "pre_hierarchy_value: " << pre_hierarchy_value
			<< " thread_local_hierarchy_value " << thread_local_hierarchy_value << endl;
		inner_mutex.unlock();
	}

	bool try_lock()
	{
		check_value();
		if (!inner_mutex.try_lock())
		{
			return false;
		}
		update_value();
		return true;
	}

private:
	std::mutex inner_mutex;
	unsigned long pre_hierarchy_value;
	unsigned long const hierarchy_value;
	static thread_local unsigned long thread_local_hierarchy_value;

	void check_value()
	{
		if (thread_local_hierarchy_value <= hierarchy_value)	//按层级加锁,要获取的锁的层级需要小于当前层级
		{
			throw std::logic_error(" invalid mutex ");
		}
	}

	void update_value()
	{
		pre_hierarchy_value = thread_local_hierarchy_value;
		thread_local_hierarchy_value = hierarchy_value;	// 传递层级值,层级减小
	}
};

thread_local unsigned long hierarchy_mutex::thread_local_hierarchy_value = ULONG_MAX;

void test5()
{
	hierarchy_mutex	m1(1000);
	hierarchy_mutex	m2(5000);

	thread t1([&]()
		{
			m1.lock();
			m1.unlock();

		});

	thread t2([&]()
		{
			lock_guard<hierarchy_mutex> lg1(m1);
		});

	t1.join();
	t2.join();
}

class Y
{
public:
	Y(int num) :_data(num)
	{
	}

	friend bool operator== (Y const& it1, Y const& it2)
	{
		if (&it1 == &it2)
		{
			return true;
		}

		int const l1 = it1.get_data();
		int const l2 = it2.get_data();
		return l1 == l2;
	}

private:
	int _data;
	mutable std::mutex _mutex;

	int get_data() const
	{
		std::lock_guard<mutex> lg(_mutex);
		return _data;
	}
};

class Singleton1
{
public:


private:
	Singleton1() {}

	static void initSingleTon()
	{
		_instance = new Singleton1();
	}

	static Singleton1* _instance;
};

void test6()
{

}

void main()
{
	test2_1();
}



