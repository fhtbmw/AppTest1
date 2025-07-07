#include "pch.h"

#include<list>
#include <vector>
#include <iterator>
#include <algorithm>
#include <assert.h>
#include <iostream>

using namespace std;

void test1()
{
	list<int> coll;

	vector<int> vtV1{2,4,5,1,34};
	vector<int> vtV2;

	vector<int>::iterator it1 = vtV1.begin();
	vector<int>::iterator it2 = vtV2.end();
	//assert(it1 != it2);

	if (vtV1.size() > 1)
	{
		vector<int>::iterator it = vtV1.begin();
		std::sort(++it, vtV1.end());
		std::sort(std::prev(it), vtV1.end());
	}

	advance(it1, 2);	//�ƶ�������
	advance(it1, -2);

	cout << *it1;
}

void test2()
{
	list<int> coll;
	for (int i = -3; i<=9; ++i)
	{
		coll.push_back(i);
	}

	list<int>::iterator pos;
	pos = find(coll.begin(), coll.end(), 2);
	if (pos != coll.end())
	{
		cout<< distance(pos, coll.end())<<endl;
	}

	list<int>::iterator it1 = coll.begin();
	list<int>::iterator it2 = coll.end();
	while (it1 != it2)
	{
		iter_swap(++it1, --it2);
	}

}

void _print(int elem)
{
	cout << elem << endl;
}

void test3()
{
	vector<int> vtV1 = {1,2,4,4,1,4,5};
	
	vector<int>::iterator pos;
	pos = find(vtV1.begin(), vtV1.end(), 5);

	vector<int>::const_reverse_iterator rpos(pos);	//���������������Ԫ��ǰһλ��

	std::for_each(vtV1.begin(), vtV1.end(), _print);
}

/* iterator_traits  ����������, ��ɻ�ȡ����������Ϣ:

value_type: ��������ָ���Ԫ�����͡�
pointer: ָ��Ԫ�ص�ָ�����͡�
reference: ��Ԫ�ص��������͡�
difference_type: ����������֮��Ĳ�ֵ���ͣ�ͨ����һ���������ͣ����ڱ�ʾ������֮��ľ��롣
iterator_category: ������������������ֲ�ͬ���͵ĵ�������������������������������ǰ��������ȣ���

*/

template<typename Y>
void _reverse(Y first, Y second)
{
	typename std::iterator_traits<Y>::difference_type n = std::distance(first, second);
	for (--n; n >0; n-=2)
	{
		typename std::iterator_traits<Y>::value_type tmp = *first;
		*first++ = *--second;
		*second = tmp;
	}
}

void test4()
{
	std::vector<int>vt1 = { 1,2,3,4,5,6 };
	_reverse(vt1.begin(), vt1.end());
}

//�Զ������������
template<typename T>
class InputIterator 
{
public:
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using difference_type = std::ptrdiff_t;
	using iterator_category = std::input_iterator_tag;

	//InputIterator(T* ptr) : ptr_(ptr);

	reference operator*()const
	{
		return *ptr_;
	}

	InputIterator& operator++()
	{
		++ptr_;
		return *this;
	}

	InputIterator operator++(int)
	{
		InputIterator temp = *this;
		++(*this);
	}

private:
	T* ptr_;
};


//function object

typedef struct fa 
{
	int id;
}F;

typedef struct son
{
	F f1;
}S;


void test5()
{
	S son1;
	F fa1;
	
	cout<<sizeof(son1)<<endl;
	cout<<sizeof(fa1)<<endl;
}

//�̳�ģ�������ģ�����������ʱѡ��ͬ�Ļ��࣬��չ����
class Base
{
public:
	
	void show()
	{
		cout<<"Base class " << name << endl;
	}

private:
	string name;
};

template<typename CC>
class temp :public CC
{
public:
	
	void show()
	{
		cout<<"temp class" << endl;
	}
};

void test6()
{
	temp<Base> t1;

	t1.show();
	int* p = new int(2);
}

int calc(int n)
{
	int res = n * 10;
	return res;
}

int add(int num, int& num2)
{
	num2 += num;
	return num;
}
void test7()
{
	vector<int> vtV1 = {1,3,4,4,23,3};
	vector<int> vtV2(6,0);

	transform(vtV1.begin(), vtV1.end(), vtV2.begin(), calc);
	transform(vtV1.begin(), vtV1.end(), ostream_iterator<int>(cout," "), calc);
}

void test8()
{
	vector<int> vtV1 = {3,4,65,4,5};

	int sum = 0;
	for_each(vtV1.begin(), vtV1.end(), 
		[&sum](int elem) {
			sum += elem;
		});
	
	for_each(vtV1.begin(), vtV1.end(), 
		[](int &n) {
			n += 1;
		});

	for_each(vtV1.begin(), vtV1.end(),
		[](int n){
			cout<<n <<" "<< endl;
		});
}

class MeanValue
{
public:
	MeanValue():num(0), sum(0)
	{

	}

	void operator() (int elem)
	{
	
	}

private:
	long num;
	long sum;
};

void main()
{
 	test8();
}


