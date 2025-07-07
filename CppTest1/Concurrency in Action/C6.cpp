#include <vector>
#include <exception>
#include <thread>
#include <stack>
#include <queue>
#include <mutex>
#include <future>
#include <condition_variable>
#include <type_traits>
#include <boost/thread.hpp>
#include <iostream>

using namespace std;

struct T
{
	T()
	{
		std::cout << " construct" << std::endl;
	}

	~T()
	{
		std::cout << " destruct" << std::endl;
	}

};

void test1()
{
	{
		T t;
	}
	std::cout << "  " << std::endl;;
}

//线程安全栈
struct empty_stack :std::exception
{
	const char* what() const throw() ;
};

template<typename T>
class threadsafe_stack
{
public:
	threadsafe_stack() {};
	threadsafe_stack(const threadsafe_stack& other) 
	{
		std::lock_guard<std::mutex> lg(other._mu);
		data = other.data;
	}
	threadsafe_stack& operator = (const threadsafe_stack&) = delete;

	void _push(T val)
	{
		std::lock_guard<std::mutex> lg(_mu);
		//data.push(val);
		data.push(std::move(val));
	}

	void _pop(T & val)
	{
		std::lock_guard<std::mutex> lg(_mu);
		if (data.empty())
		{
			throw empty_stack();
		}
		val = std::move(data.top());
		data.pop();
	}

	std::shared_ptr<T> pop()
	{
		std::lock_guard<std::mutex> lg(_mu);
		if (data.empty())
		{
			throw empty_stack();
		}
		std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
		data.pop();
		return res;
	}

	bool empty() const
	{
		std::lock_guard<std::mutex> lk(_mu);
		return data.empty();
	}

private:
	std::stack<T> data;
	mutable std::mutex _mu;
};

std::once_flag f1;

void func1()
{
	cout << "func1" << endl;
}

void _call_once()
{
	std::call_once(f1, func1);
}

void test2()
{
	thread t1(_call_once);
	thread t2(_call_once);

	t1.join();
	t2.join();
}

class _thread
{
public:
	explicit _thread(std::thread& t):m_thread(t) {};

	~_thread()
	{
		if (m_thread.joinable())	//析构时自动将线程join
		{
			m_thread.join();
		}
	}

private:
	std::thread& m_thread;
};

void test3()
{
	packaged_task<double(int, int)>	 task([](int a, int b) 
		{
			cout <<  "task" << this_thread::get_id() << endl;
			return pow(a, b);
		});

	std::future<double>future = task.get_future();
	
	std::thread t1(std::bind(std::move(task), 10 ,2));
	_thread tt(t1);

	cout << this_thread::get_id() << endl;
	std::cout << future.get() << '\n';
}

void func2(int n)
{}

template<typename T>
typename std::enable_if<std::is_integral<T>::value, void>::type
print_int(T va)		//传入的参数类型 为指定类型，函数才有效
{
	cout << va << endl;
}


void test4()
{
	std::cout << std::boolalpha;
	std::cout << std::is_invocable_v<decltype(func2), std::string> << std::endl;	//编译时期判断是否可按照指定参数调用函数
	std::cout << std::is_invocable_v<decltype(func2), double> << std::endl;		//double可隐式转换为int类型
	
	int i = 1;
	print_int(i);
}

template<typename T>
class thread_queue
{
public:
	thread_queue() {};

	bool _empty()
	{
		std::unique_lock<std::mutex> lk(_mu);
		return _queue.empty();
	}

	void wait_and_pop(T &val)
	{
		std::unique_lock<std::mutex> lk(_mu);
		_cv.wait(lk, []()
			{
				return !_empty();
			});
		val = std::move(*_queue.front());
		_queue.pop();
	}
	
	std::shared_ptr<T> wait_and_pop()
	{
		std::unique_lock<std::mutex> lk(_mu);
		_cv.wait(lk, []()
			{
				return !_empty();
			});
		std::shared_ptr<T> res = _queue.front();
		_queue.pop();
	}

	void _push(T &val)
	{
		std::lock_guard<std::mutex> lg(_mu);
		_queue.push(std::make_shared<T>(std::move(val)));
		_cv.notify_one();
	}

private:
	std::mutex _mu;
	std::queue<std::shared_ptr<T>> _queue;
	std::condition_variable _cv;
};

template<typename T>
class _node_queue
{
public:
	_node_queue() {};

	_node_queue(const _node_queue& other) = delete;

	_node_queue& operator = (const _node_queue& other) = delete;

	void push(T val)
	{
		std::unique_ptr<node> p(new node(std::move(val)));
		node* const new_tail = p.get();
		if (tail)
		{
			tail->next = std::move(p);
		}
		else
		{
			head = std::move(p);
		}
		tail = new_tail;
	}

private:
	struct node
	{
		node(T t) :_data(std::move(t)) {}

		T _data;
		std::shared_ptr<node> next;
	};

	std::unique_ptr<node> head;
	node* tail;
};

template<typename T>
class _node_queue2
{
public:
	_node_queue2():head(new node), tail(head.get()) {}

	_node_queue2(const _node_queue2& other) = delete;

	_node_queue2& operator = (const _node_queue2& other) = delete;

	void push(T val)
	{
		std::shared_ptr<T> new_data(std::make_shared<T>(std::move(val)));
		std::unique_ptr<node> p(new node);
		tail->_data = new_data;
		node* const new_tail = p.get();	//新的虚拟尾节点
		tail->_next = std::move(p);		//指向新的尾节点
		tail = new_tail;
		std::cout << val << std::endl;
	}

	std::shared_ptr<T> try_pop()
	{
		if (head.get() == tail)
		{
			return std::shared_ptr<T>();
		}
		std::shared_ptr<T> const res(head->_data);
		std::unique_ptr<node> old_head = std::move(head);
		head = std::move(old_head->_next);
		std::cout << " pop " << *res << std::endl;
		return res;
	}

private:
	struct node
	{
		std::shared_ptr<T> _data;
		std::unique_ptr<node> _next;
	};

	std::unique_ptr<node> head;
	node* tail;
};

int func3(int n)
{
	std::cout << n << std::endl;
	return n * 10;
}

template<typename T>
class _node_queue3
{
public:
	_node_queue3():head(new node), tail(head.get()) {};

	_node_queue3(const _node_queue3& item) = delete;

	_node_queue3& operator = (const _node_queue3& itme) = delete;

	std::shared_ptr<T> pop()
	{
		std::shared_ptr<node> item = pop_head();
		return item ? item->_data: std::shared_ptr<T>();
	}

	void push(T val)
	{
		std::shared_ptr<T> data(std::make_shared<T>(std::move(val)));
		std::unique_ptr<node> p(new node);
		node* const new_tail = p.get();
		std::lock_guard<std::mutex> lg(tail_mutex);
		tail->_data = data;
		tail->_next = std::move(p);
		tail = new_tail;
		std::cout << " push " << *data << std::endl;
	}

private:
	struct node
	{
		std::shared_ptr<T> _data;
		std::unique_ptr<node> _next;
	};
	std::unique_ptr<node> head;
	node* tail;
	std::mutex head_mutex;
	std::mutex tail_mutex;

	node* get_tail()
	{
		std::lock_guard<std::mutex> lg(tail_mutex);
		return tail;
	}

	std::unique_ptr<node> pop_head()
	{
		std::lock_guard<std::mutex> lg(head_mutex);
		if (head.get() == tail)
		{
			return std::unique_ptr<node>();
		}
		std::unique_ptr<node> old_head = std::move(head);
		std::cout << " pop " << * (old_head->_data) << std::endl;
		head = std::move(old_head->_next);
		return old_head;
	}
};


void test5()
{
	/*std::invoke(func3, 2);
	std::unique_ptr<int> up(new int(2));*/

	/*_node_queue2<int> que1;
	que1.push(1);
	que1.push(2);
	que1.push(3);

	que1.try_pop();
	que1.try_pop();*/

	_node_queue3<int> que2;
	std::thread t1(std::bind(&_node_queue3<int>::push, &que2, 1));
	std::thread t2(std::bind(&_node_queue3<int>::push, &que2, 2));
	std::thread t3(std::bind(&_node_queue3<int>::pop, &que2));

	t1.join();
	t2.join();
	t3.join();
}



void test6()
{

}

void main()
{
	test5();
}




