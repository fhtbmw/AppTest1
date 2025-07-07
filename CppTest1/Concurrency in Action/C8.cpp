#include <mutex>
#include <future>
#include <vector>
#include <array>
#include <numeric>
#include <algorithm>
#include <random>
#include <atomic>
#include <thread>
#include <assert.h>
#include <iostream>

template<typename Iterator, typename T>
T parrallel_accumulate(Iterator it, Iterator it2, T init)
{
	unsigned long const length = std::distance(it, it2);	//元素数量
	unsigned long const max_chunk_size = 20;	//设置并发处理的阈值
	if (length <= max_chunk_size)
	{
		return std::accumulate(it, it2, init);
	}
	else
	{
		Iterator mid_data = it;
		std::advance(mid_data, length / 2);
		std::future<T> first_ret = std::async(parrallel_accumulate<Iterator, T>, it, mid_data, T());

		T second_ret = parrallel_accumulate(mid_data, it2, T());
		return first_ret.get() + second_ret;
	}
}

//并行版accumulate
template<typename T>
void test1()
{
	std::vector<T> vtN1;
	std::default_random_engine dre;
	std::mt19937 mt;
	std::uniform_int_distribution di(1,10000);

	for (int i = 0; i<3000; i++)
	{
		vtN1.push_back(di(mt));
	}

	auto start_t1 = std::chrono::system_clock::now();
	T ret = parrallel_accumulate(vtN1.begin(), vtN1.end(), 0);
	auto end_t1 = std::chrono::system_clock::now();
	std::cout << " accumulate ret: " << ret << std::endl;
	std::cout <<std::chrono::duration<double>(end_t1 - start_t1).count()<< std::endl;

	auto start_t2 = std::chrono::system_clock::now();
	T ret2 = std::accumulate(vtN1.begin(), vtN1.end(), 0);
	auto end_t2 = std::chrono::system_clock::now();
	std::cout << std::chrono::duration<double>(end_t2 - start_t2).count() << std::endl;

}

void test2()
{
	/*std::random_device rd;
	std::mt19937 generator(rd());
	std::cout << generator() << std::endl;*/
	
}

void test3()
{
	std::vector<int>vt1;
	std::array<int, 5> arr1 = {};
	
	
}

class join_threads
{
	std::vector<std::thread>& threads;
public:
	explicit join_threads(std::vector<std::thread>& threads_) : threads(threads_)
	{}

	~join_threads()
	{
		for (unsigned long i = 0; i<threads.size(); i++)
		{
			if (threads[i].joinable())
			{
				threads[i].join();
			}
		}
	}

};


//并行 find
template<typename Iterator, typename MatchType>
Iterator parallel_find(Iterator first, Iterator last, MatchType macth)
{
	struct find_element
	{
		void operator()(Iterator begin, Iterator end, MatchType match,
						std::promise<Iterator>* result,
						std::atomic<bool>* done_flag)
		{
			try
			{
				for (; (begin != end) && !done_flag->load(); ++begin)
				{
					if ( *begin == match)
					{
						result->set_value(begin);	//匹配成功，在promise中设置
						done_flag->store(true);
						return;
					}
				}
			}
			catch (...)
			{
				try
				{
					result->set_exception(std::current_exception());
					done_flag->store(true);
				}
				catch (...)
				{
				}
			}
		}
	};

	unsigned long const length = std::distance(first, last);

	if (!length)
	{
		return last;
	}

	unsigned long const min_per_thread = 25;
	unsigned long const max_threads = (length + min_per_thread - 1) / min_per_thread;

	unsigned long const hardware_threads = std::thread::hardware_concurrency();

	unsigned long const num_threads = std::min(hardware_threads != 0 ? hardware_threads : 2, max_threads);		
	unsigned long const block_size = length / num_threads;

	std::promise<Iterator> result;
	std::atomic<bool> done_falg(false);
	std::vector<std::thread> threads(num_threads - 1);
	
	{
		join_threads joiner(threads);

		Iterator block_start = first;
		for (unsigned long i = 0; i < (num_threads - 1); ++i)	//使用线程执行find_element
		{
			Iterator block_end = block_start;
			std::advance(block_end, block_size);
			threads[i] = std::thread(find_element(), block_start,
									block_end, macth,
									&result, &done_falg);
			block_start = block_end;
		}
		find_element()(block_start, last, macth, &result, &done_falg);
	}
	if ( !done_falg.load())
	{
		return last;
	}
	return result.get_future().get();

}


//
template<typename Iterator, typename MatchType>
Iterator parallel_find_impl(Iterator first, Iterator last, MatchType match,
							std::atomic<bool>& done)
{
	try
	{
		unsigned long const length = std::distance(first, last);
		unsigned long const min_per_thread = 25;
		if (length < 2 * min_per_thread)
		{
			for (; (first != last) && !done.load(); ++first)
			{
				if (*first == match)
				{
					done = true;
					return first;
				}
			}
			return last;
		}
		else
		{
			Iterator const mid_point = first + (length / 2);
			std::future<Iterator> fu1 = std::async(&parallel_find_impl<Iterator, MatchType>,
				mid_point, last, match, std::ref(done));
			Iterator const direct_result = parallel_find_impl(first, mid_point, match, done);
			return (direct_result == mid_point) ? fu1.get() : direct_result;
		}

	}
	catch (...)
	{
		done = true;
		throw;
	}
}


void test4()
{
	std::vector<int> vtN1{0,3,21,3};
	std::vector<int> vtN2;
	
	std::default_random_engine dre;
	std::uniform_int_distribution uid(10, 2000);
	for (int i = 0; i<10000; i++)
	{
		vtN2.push_back(uid(dre));
	}
	int target = vtN2[99];
	std::atomic<bool> flag;

	auto starttime1 = std::chrono::system_clock::now();
	auto ret = parallel_find<std::vector<int>::iterator, int>(vtN1.begin(), vtN1.end(), target);
	auto endtime1 = std::chrono::system_clock::now();

	auto starttime2 = std::chrono::system_clock::now();
	auto ret2 = parallel_find_impl<std::vector<int>::iterator, int>(vtN1.begin(), vtN1.end(), target, flag);
	auto endtime2 = std::chrono::system_clock::now();

	std::cout << " " << std::chrono::duration<double>( endtime1- starttime1).count() << std::endl;
	std::cout << " " << std::chrono::duration<double>( endtime2- starttime2).count() << std::endl;

}

//interruptable_thread
class interrupt_flag
{
public:
	void set() {};
	bool is_set() const {};

};
thread_local interrupt_flag this_thread_interrupt_flag;

class interruptile_thread
{
	std::thread internal_thread;
	interrupt_flag* flag;

public:
	template<typename FunctionType>
	interruptile_thread(FunctionType f)
	{
		std::promise<interrupt_flag*>p;
		internal_thread = std::thread([f, &p]()		//线程持有 f副本、promise变量的引用
			{
				p.set_value(&this_thread_interrupt_flag);
				f();
			});
		flag = p.get_future().get();
	}

	void interrupt()
	{
		if (flag)
		{
			flag->set();
		}
	}
};

bool x = false;
std::atomic<bool> y;
std::atomic<int> z;

void write_x_then_y()
{
	x = true;	//栅栏前存储x
	std::atomic_thread_fence(std::memory_order_release);
	y.store(true, std::memory_order_relaxed);	//栅栏后存储y
}

void read_y_then_x()
{
	while (!y.load(std::memory_order_relaxed));
	std::atomic_thread_fence(std::memory_order_acquire);
	if (x)
		++z;
}

class interupt_flag2
{
	std::atomic<bool>flag;
	std::condition_variable* thread_cond;
	std::mutex	set_clear_mutex;
public:
	interupt_flag2():thread_cond(0)
	{}

	void set()
	{
		flag.store(true, std::memory_order_relaxed);
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		if (thread_cond)
		{
			thread_cond->notify_all();
		}
	}

	bool is_set() const
	{
		return flag.load(std::memory_order_relaxed);
	}

	void set_condition_variable(std::condition_variable& cv)
	{
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		thread_cond = &cv;
	}

	void clear_condition_variable()
	{
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		thread_cond = 0;
	}
};

class interrupt_flag3
{
	std::atomic<bool> flag;
	std::condition_variable* thread_cond;
	std::condition_variable_any* thread_cond_any;
	std::mutex set_clear_mutex;

public:
	interrupt_flag3() :thread_cond(0), thread_cond_any(0)
	{}

	void set()
	{
		flag.store(true, std::memory_order_relaxed);
		std::lock_guard<std::mutex> lk(set_clear_mutex);
		if (thread_cond)
		{
			thread_cond->notify_all();
		}
		else if (thread_cond_any)
		{
			thread_cond_any->notify_all();
		}
	}

	template<typename lockable>
	void wait(std::condition_variable_any& cv, lockable& lk)
	{
		struct custom_lock
		{
			interrupt_flag3* self;
			lockable& lk;

			custom_lock(interrupt_flag3* self_, std::condition_variable_any& cond, lockable& lk_):
				self(self_), lk(lk_)
			{
				self->set_clear_mutex.lock();
				self->thread_cond_any = &cond;
			}

			void unlock()
			{
				lk.unlock();
				self->set_clear_mutex.unlock();
			}

			void lock()
			{
				std::lock(self->set_clear_mutex, lk);
			}

			~custom_lock()
			{
				self->thread_cond_any = 0;
				self->set_clear_mutex.unlock();
			}
		};
	}


};

struct X
{

};

void func1(X&& x)
{
	
}

void test5()
{
	//interrupt_flag3 in3;
	std::vector<int> vtN1{ 2,3,2 };
	std::vector<int> vtN2;
	int array[2] = { 2,2 };
	std::copy(array, array + 2, vtN1.begin());
	//std::copy(array, array + 2, vtN2.begin());	//error std::copy 只copy到已有元素的容器
	std::copy(array, array + 2, back_inserter(vtN2));	// ok

 	X();
	func1(X());
}

template<typename A>
void func2(A&& a)	//模板参数为&&，既可接受左值(解析为左值引用)，也可接受右值(解析为普通函数)
{
}

void func3(int && i)
{
	
}

void test6()
{
	func2<std::string>("Dasha's");
	func2("d");
	func2<int>(2);

	int i = 0;
	func2(i);

	func3(2);
	// func3(i)  //error 
	func3(std::move(i));
}

int g_num = 1;
const int g_num2 = 1;

template<typename T, int size>	// size 非类型模板参数
void func4()
{
	/*const int cin1;
	constexpr int cin2;*/		// error 两种类型都需初始化
}

void test7()
{
	func4<double, g_num2>();	// 常量 | 常量表达式作为非类型模板参数
	//func4<double, g_num>();	// error
}

void main()
{
	test7();
}















