#include "pch.h"
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <thread>
#include <functional>
using namespace std;

template<typename T>
void _Out(T t)
{
	cout << t << endl;
}

void test1()
{
	// 随机数生成器
	std::default_random_engine	dre;

	std::uniform_int_distribution<int> di(10, 20);
	for (int i = 0; i < 20; ++i)
	{
		std::cout << di(dre) << " ";
	}
	std::cout << std::endl;

	std::uniform_real_distribution<double> dr(10, 20);
	for (int i = 0 ; i < 8 ; ++ i)
	{
		cout << dr(dre) << endl;
	}

	std::random_device rd;
	std::mt19937 mt(rd());


	std::vector<int> v = { 2,3,5,4,45 };
	std::shuffle(v.begin(), v.end(), rd);
	std::for_each(v.begin(), v.end(), _Out<int>);


}

struct move_only
{
	move_only()
	{
		std::puts("default construct");
	}

	move_only(const move_only&) = delete;

	move_only(move_only&&)noexcept
	{
		std::puts("move construct");
	}
	
};

void f(move_only) {};

void test2()
{
	move_only mv;
	
	std::thread t{ f, std::move(mv) };
	t.join();
}

void main()
{	
	test2();
}	



