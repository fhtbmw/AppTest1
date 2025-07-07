#include<boost/noncopyable.hpp>
#include <iostream>
#include<boost/optional.hpp>
#include<boost/typeof/typeof.hpp>
#include<vector>
#include<boost/assign.hpp>
#include<map>
#include<ctime>

typedef boost::noncopyable noncopyable;

using namespace std;
using namespace boost;

/*
noncopyable类将拷贝构造函数、operator= 设为私有属性,子类继承后禁止了拷贝、赋值行为
*/
class Test :noncopyable {

};

void Test1(){
	Test t1;
	Test t2;
	//Test t3 (t1);		//错误
}

/*

optional,可当作是一个智能指针,作用广泛 

*/

void OptionalTest() {
	optional<int>op1;
	vector<int>v1(10);
	optional<vector<int>&>op2(v1);
	op2->push_back(2);
	for (auto it = op2->begin(); it != op2->end();it++) {
		//cout<<*it<<" ";
	}

	BOOST_AUTO(n,make_optional(2));
}

//作为函数返回值
optional<int>compute(int x) {
	//int t = x - 1;
	return optional<int>(x>0,x-1);
}

/*

assign库，可以简易地对STL中的容器进行操作

*/

/*

assign 提供辅助函数 push_back() push_front insert(),它们可作用于拥有同名函数的容器
map_list_of  pair_list_of  list_of()用于构造容器时快速填充数据

*/

void AssignTest() {
	using namespace boost::assign;
	vector<char>cv;
	
	cv += '2', '23', 'dasd';
	map<int, string>map1;
	map1 += make_pair(1, "a"), make_pair(2, "b");	//map容器需要make_pair生成新元素
	push_back(cv)(2)(3)(1223213113)('asds');		//使用assign库下的函数
	insert(map1)(3,"c")(4,"d");				//使用assign库函数，简化写法

	set<string>ss=list_of("dasd")("dsadad")("dsdsd");
	//set<string>sss=(list_of("dasd"),"dsadad","dsdsd");  //结合逗号写法
	
	map<int,string>mi = map_list_of(1,"aa")(2,"bb");

}

/* 

支持容器适配器 

*/

/*

容器适配器没有inert、push_back等标准容器的成员函数，assign使用时需在初始化时填充数据；
且需加上.to_adapter()用以适配到容器适配器。

*/

void AssignTest2()
{
	using namespace boost::assign;
	stack<int> si = list_of(3)(3)(4).to_adapter();
	while (!si.empty())
	{
		cout<<si.top()<<endl;
		si.pop();
	}
}

void PrintTime() {
	time_t now = time(nullptr);
	char time[26];
	ctime_s(time,sizeof(time), &now);
	cout<<"********"<<time<<"********" << endl;
}

/*

boost 库中的swap函数;
std 库中的swap函数对于自定义类型来说，执行效率低(内部进行拷贝构造、赋值等操作)

*/

void SwapTest() {
	int n1[10];
	int n2[10];
	fill_n(n1,10,1);
	fill_n(n2,10,2);
	boost::swap(n1,n2);
}

void main()
{
	Test1();
}
