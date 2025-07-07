#include <mutex>
#include <vector>
#include <condition_variable>
#include <queue>
#include <memory>
#include <future>
#include <utility>
#include <thread>
#include <list>
#include <iostream>

using namespace std;


//同步并发

bool g_flag;
std::mutex m;

void wait_for_falg()
{
	std::unique_lock<std::mutex> lk(m);
	while (!g_flag)
	{
		lk.lock();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		lk.unlock();
	}
}

void data_preparation_thread()
{
	condition_variable cv;
}

template<typename T>
class threadsafe_quque
{
public:
	threadsafe_quque()
	{}

	threadsafe_quque(threadsafe_quque const & other)
	{
		std::lock_guard<std::mutex> lg(_mutex);
		this->_queue = other._queue;
	}

	void push(T value);
	void waitandpop(T &value);
	std::shared_ptr<T> waitandpop();
	bool trypop(T& value);
	std::shared_ptr<T> trypop();
	bool empty()const;

private:
	std::mutex _mutex;
	std::queue<T> _queue;
	std::condition_variable cv;
};

template<typename T>
void threadsafe_quque<T>::push(T t)
{
	std::unique_lock<std::mutex> lk(_mutex);
	_queue.push(t);
	cout << " push " << t << endl;
	cv.notify_one();
}

template<typename T>
void threadsafe_quque<T>::waitandpop(T& value)
{
	std::unique_lock<std::mutex> lk(_mutex);
	cv.wait(lk, [this]()
		{
			return !_queue.empty();
		});
	value = _queue.front();
	_queue.pop();
}

template<typename T>
std::shared_ptr<T> threadsafe_quque<T>::waitandpop()
{
	std::unique_lock<std::mutex> lk(_mutex);
	cv.wait(lk, [this]()
		{
			return !_queue.empty();
		});
	std::shared_ptr<T> p(std::make_shared<T>(_queue.front()));
	_queue.pop();
	return p;
}

template<typename T>
bool threadsafe_quque<T>::trypop(T& v)
{
	std::lock_guard<std::mutex> lg(_mutex);
	if (_queue.empty())
	{
		return false;
	}
	v = _queue.front();
	_queue.pop();
	return true;
}

template<typename T>
std::shared_ptr<T> threadsafe_quque<T>::trypop()
{
	std::lock_guard<std::mutex> lg(_mutex);
	if (_queue.empty())
	{
		return shared_ptr<T>();
	}
	std::shared_ptr<T> p(std::make_shared<T>(_queue.front()));
	_queue.pop();
	cout << "pop " << endl;
	return p;
}

template<typename T>
bool threadsafe_quque<T>::empty()const
{
	std::lock_guard<std::mutex> lg(_mutex);	
	return _queue.empty();
}

void test1()
{
	threadsafe_quque<int> q1;
	thread t1([&]() 
		{
			q1.push(2);
			q1.push(3);
			q1.push(3);
			q1.push(3);
		});

	thread t2([&]()
		{
			//q1.waitandpop();
			//while (!q1.empty())		//error
			//{
			//	q1.trypop();
			//}
			q1.trypop();
		});

	t1.join();
	t2.join();
}

class Y
{
public:
	void funcy(int n) 
	{
		cout << "funcy" << endl;
	}
};

void test2()
{
	Y y;
	std::future<void> f1 = std::async(&Y::funcy, y, 2);
	std::future<void> f2 = std::async(std::launch::async, bind(&Y::funcy, y, 2));

}


std::mutex m2;
std::deque<std::packaged_task<void()>> tasks;

bool shutdown_message_received()
{
	cout << "shutdown message " << endl;
	return true;
}

void getandproessmessage()
{
	cout << "getandprocess message" << endl;
}

void gui_thread()
{
	while (!shutdown_message_received())
	{
		getandproessmessage();
		std::packaged_task<void()> task;
		{
			std::lock_guard<std::mutex> lk(m2);
			if (tasks.empty())
			{
				continue;
			}
			task = std::move(tasks.front());
			tasks.pop_front();
		}
		task();
	}
}

template<typename Func>
std::future<void> post_task_for_gui_thread(Func f)
{
	std::packaged_task<void()> task(f);
	std::future<void> res = task.get_future();
	std::lock_guard<std::mutex> lk(m);
	tasks.push_back(std::move(task));
	return res;
}

bool wait_loop()
{
	std::mutex m;
	condition_variable cv;
	bool flag = false;

	auto wait_time = std::chrono::steady_clock::now() + std::chrono::microseconds(1000);
	std::unique_lock<std::mutex> lk(m);
	while (!flag)
	{
		if (cv.wait_until(lk, wait_time) == std::cv_status::timeout)
		{
			flag = false;
			break;
		}
	}
	return flag;
}

class thread_guard
{
public:
	explicit thread_guard(std::thread& t) :m_t(t)
	{
		
	}

	~thread_guard()
	{
		std::puts(" ~ thread_guard");
		if (m_t.joinable())
		{
			m_t.join();
		}
	}

private:
	std::thread& m_t;
};

void func(int & num)
{
	cout << "func input data adderss: " << &num << endl;
}


void test3()
{
	//std::thread _thread(gui_thread);
	std::atomic_bool flag;
	

	wait_loop();
	int n = 0;
	
	std::thread t;
	thread_guard tg(t);

	cout << "n adderss " << &n << endl;

	//thread t2(bind(func, std::ref(n)));	//隐式转换为引用类型，传入函数的为引用类型
	thread t2(bind(func,n));				//传入func的为变量的拷贝
	t2.join();

}

void main()
{
	test3();
}

 