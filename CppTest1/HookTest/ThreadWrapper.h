#pragma once
#ifndef _THREADWRAPPER_H_
#define _THREADWRAPPER_H_

#include <thread>

using namespace std;

class _thread
{
public:
	_thread()
	{
		
	}

	_thread(const _thread& t) = delete;

	_thread& operator= (const _thread& t) = delete;

	template<typename T, typename... Args>
	_thread(T&& func, Args&&... args): t(std::forward<T>(func), std::forward<Args>(args)...)
	{
	}

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



#endif //! _THREADWRAPPER_H_
