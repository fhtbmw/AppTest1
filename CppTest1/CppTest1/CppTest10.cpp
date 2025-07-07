#include "pch.h"

#include<locale>
#include <vector>
#include <algorithm>
#include <iostream>
#include<algorithm>
#include <string>

using namespace std;

void test_Lambda()
{
	auto plus1 = [](int i) {
		return i+1;
	};

	cout<<"plus1 "<< plus1(1) << endl;
}

void test_MeanValue()
{
	vector<int>vtV1 = {1,3,4,1,3,3,4,43};
	
	int sum = 0;
	auto func = [&sum](int i){
		sum += i;
	};

	for_each(vtV1.begin(), vtV1.end(), func);

	auto meanvalue = static_cast<double>(sum) / static_cast<double>(vtV1.size());

	cout<<meanvalue<<endl;
}

char myFunc(char c)
{
	std::locale loc;
	return std::use_facet<std::ctype<char>>(loc).toupper(c);
}

void test_Lambda2()
{
	string s("HANDSUP");
	string ss("HAN");

	auto func = [](char c1, char c2) {
		return myFunc(c1) == myFunc(c2);
		};

	string::iterator pos;
	pos = search(s.begin(), s.end(), ss.begin(), ss.end(),func);

	if (pos != s.end())
	{
		cout << "ss is a part of s" << endl;
	}

 }

class Trucks
{
public:
	Trucks(const string& s):_name(s){}

	void Show() const 
	{
		cout << _name << endl;
	}

private:
	string _name;
};

template<typename T>
void funcTruck(T t)
{
	t.Show();
}

class Car
{
public:
	Car(const string & n):_name(n) {}

	void Show()
	{
		cout << _name << endl;
	}
private:
	string _name;
};

//继承模板参数，模板类可在运行时选择不同的基类，扩展功能
template<typename _Type>
class Tool : public _Type
{
public:
	Tool(const string& n):Trucks(n) {};		//调用基类构造函数
};

template<typename _T>
auto funcShowTool = [](const Tool<_T> item) {
	item.Show();
};

void test_Lambda3()
{
	vector<Trucks> vtTruck = {Trucks("BENZ"), Trucks("SCNAIA"), Trucks("VOLVO")};
	std::for_each(vtTruck.begin(), vtTruck.end(),	
		[](Trucks item) {
			item.Show();
		});

	vector<Tool<Trucks>> vtTool1 = {Tool<Trucks>("MAN"), Tool<Trucks>("BYD"), Tool<Trucks>("NISSAN") };

	std::for_each(vtTool1.begin(), vtTool1.end(), funcShowTool<Trucks>);	// for_each 对区间内的每个元素调用函数		
}

template<typename T>
T func(T& a)
{
	a += 1;
	return a;
}

void test4()
{
	vector<int>a{2,4,5,5};
	std::for_each(a.begin(), a.end(), func<int>);
	
	int num1 = 2;
	int& num2 = num1;
	cout<<func(num2);

}

void main()
{
	test4();
}






