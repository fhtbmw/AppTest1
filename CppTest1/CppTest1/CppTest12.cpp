#include "pch.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <cassert>
#include <list>
#include <numeric>
#include <deque>
#include <stack>
#include <queue>
#include <bitset>

using namespace std;

void test1()
{
	stack<int>sNum1;

	priority_queue<int> q1;
	q1.push(1);
	q1.push(2);
	q1.push(3);

	while (!q1.empty())
	{
		auto item = q1.top();
		cout << item <<endl;
		q1.pop();
	}

}


void test2()
{
	std::bitset<8> b1;

	enum  e1
	{
		a,c,d,f,e,g		//enum 默认从0递增,每个元素都有值
	};

	std::bitset<f>b2;
	b2.set(a);			//set: 置标志位为1
	b2.set(d);

	size_t Count = b2.count();	//返回标志位为1的个数

}

void main()
{
	test2();
}









