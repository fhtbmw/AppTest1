#include <vector>
#include <atomic>
#include <thread>
#include <functional>
#include <iostream>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <assert.h>

std::vector<int> vtdata;
std::atomic<bool> data_ready(false);
std::mutex mu1;
std::condition_variable cv1;

bool flag1 = false;
bool flag2 = false;
int g_num1 = 0;

void reader_thread()
{
	while (! data_ready.load())
	{
		std::this_thread::yield();
	}

	if (! vtdata.empty())
	{
		std::cout << " reader data " << g_num1 << std::endl;
	}
	else
	{
		std::cout << " data empty" << std::endl;
	}

	data_ready.store(true, std::memory_order_seq_cst);

}

void write_thread()
{
	while ( data_ready)
	{
		std::this_thread::yield();
	}
	g_num1++;
	data_ready.store(false, std::memory_order_seq_cst);
}

std::atomic<bool> x, y;
std::atomic<int> z;

void write_x()
{
	x.store(true, std::memory_order_seq_cst);	// 1
}

void write_y()
{
	y.store(true, std::memory_order_seq_cst);	// 2
}

void read_x_then_y()
{
	while (! x.load(std::memory_order_seq_cst));
	if (y.load(std::memory_order_seq_cst))		// 3
	++z;
}

void read_y_then_x()
{
	while (!y.load(std::memory_order_seq_cst));
	if (x.load(std::memory_order_seq_cst))		// 4
	++z;
}

void test2()
{
	/*std::thread t1(std::bind(write_thread));
	std::thread t2(std::bind(reader_thread));

	t1.join();
	t2.join();*/

	x = false;
	y = false;
	z = 0;
	
	std::thread a(write_x);
	std::thread b(write_y);
	std::thread c(read_x_then_y);
	std::thread d(read_y_then_x);
	a.join();
	b.join();
	c.join();
	d.join();
	assert(z.load() != 0);	// 5
}

std::atomic<int> _x(0), _y(0), _z(0);
std::atomic<bool> go(false);

struct read_data
{
	int x, y, z;
};

read_data val1[10];
read_data val2[10];
read_data val3[10];
read_data val4[10];
read_data val5[10];

void increment(std::atomic<int>* var_to_inc, read_data* data)
{
	while (! go)
	{
		std::this_thread::yield();
	}

	for (unsigned i = 0; i < 10; i++)
	{
		data[i].x = _x.load(std::memory_order_relaxed);
		data[i].y = _y.load(std::memory_order_relaxed);
		data[i].z = _z.load(std::memory_order_relaxed);

		var_to_inc->store(i + 1, std::memory_order_relaxed);
		std::this_thread::yield();
	}
}

void read_values(read_data* data)
{
	while (!go)
	{
		std::this_thread::yield();
	}
	for (unsigned i =0; i < 10; i++)
	{
		data[i].x = _x.load(std::memory_order_relaxed);
		data[i].y = _y.load(std::memory_order_relaxed);
		data[i].z = _z.load(std::memory_order_relaxed);
		std::this_thread::yield();
	}
}

void print(read_data* data)
{
	for (unsigned i =0; i < 10 ; i++)
	{
		if (i)
		{
			std::cout << " " << data[i].x << " " << data[i].y << " " << data[i].z << std::endl;
		}
	}
}

void test3()
{
	std::thread t1(increment, &_x, val1);
	std::thread t2(increment, &_y, val2);
	std::thread t3(increment, &_z, val3);
	
	std::thread t4(read_values, val4);
	std::thread t5(read_values, val5);

	go = true;

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();

	print(val1);
	print(val2);
	print(val3);
	print(val4);
	print(val5);

}

class _spin_lock
{
public:
	_spin_lock() noexcept = default;

	void lock() noexcept
	{
		while (f.test_and_set(std::memory_order_acquire));	//设置原子量状态 ->true
		{

		}
	}

	void unlock()
	{
		f.clear(std::memory_order_release);		//清除状态 ->false
	}

private:
	std::atomic_flag f;
};

int g_num = -1;
_spin_lock splk;

class _thread
{
public:
	_thread(std::thread& t) :_t(t) {};

	~_thread()
	{
		if (_t.joinable())
		{
			_t.join();
		}
	}

private:
	std::thread& _t;
};

void func1()
{
	std::lock_guard<_spin_lock> lg(splk);
	g_num++;
	
	std::cout << std::this_thread::get_id() << " " << g_num << std::endl;
}

void test4()
{
	std::thread t1(std::bind(func1));
	std::thread t2(std::bind(func1));

	_thread t11(std::ref(t1));
	_thread t22(std::ref(t2));

}

void test5()
{
	std::atomic<bool> ab{true};

	bool b1 = ab.load(std::memory_order_acquire);
	ab.store(false);

	b1 = ab.exchange(false, std::memory_order_relaxed);	// 原子值和参数值一致，存储到b1中	
}

struct Foo {};

Foo array[5]{};
std::atomic<Foo*> p{ array };

Foo* pf = p.fetch_add(1);		//在存储地址上加1,相当于后移指针

Foo* pf2 = p.fetch_sub(2);


void test6()
{
	data_ready = true;

	std::thread t1(std::bind(reader_thread));
	std::thread t2(std::bind(write_thread));
	std::thread t3(std::bind(write_thread));
	std::thread t5(std::bind(write_thread));
	std::thread t6(std::bind(write_thread));
	std::thread t4(std::bind(reader_thread));
	std::thread t7(std::bind(reader_thread));

	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	t6.join();
	t7.join();
}


struct MyStruct1
{
public:
	long ldata1;
	long ldata3;
	long ldata4;
	long ldata5;
	long ldata6;
	long ldata7;
	long ldata8;
	long ldata9;
	long ldata2;
};

void test7()
{
	MyStruct1 m1;

	std::thread t1([&]()
		{
			for (int i = 0; i < 100000000; i++)
			{
				m1.ldata1++;
			}
		});

	std::thread t2([&]()
		{
			for (int i = 0; i < 100000000; i++)
			{
				m1.ldata2++;
			}
		});

	t1.join();
	t2.join();
}

void main()
{
	auto start_time = std::chrono::system_clock::now();
	test7();
	auto end_time = std::chrono::system_clock::now();
	std::cout << std::chrono::duration<double>(end_time - start_time) << std::endl;
}





