#pragma once
#ifndef _THREADWRAPPER_H_
#define _THREADWRAPPER_H_

#include <thread>
#include <functional>

using namespace std;

class _thread
{
public:
	_thread()
	{
		
	}

	template<typename T, typename... Args>
	_thread(T&& func, Args&&... args): t(std::forward<T>(func), std::forward<Args>(args)...)
	{
	}

	_thread(const _thread& t) = delete;

	_thread& operator= (const _thread& t) = delete;

	~_thread()
	{
		if (t.joinable())
		{
			t.join();
		}
	}

private:
	std::thread t;
};

class _thread2
{
public:

	typedef std::function<void* (void*)> Fun;

	_thread2(Fun f) :_t(f)
	{

	}

	_thread2(const _thread2& t) = delete;

	_thread2& operator= (const _thread2& t) = delete;

	~_thread2()
	{
		if (_t.joinable())
		{
			_t.join();
		}
	}

private:

	thread _t;

};



#endif //! _THREADWRAPPER_H_
