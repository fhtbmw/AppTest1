#include<boost/function.hpp>
#include<boost/ref.hpp>
#include<boost/typeof/typeof.hpp>
#include<boost/utility/result_of.hpp>
#include<boost/bind.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

/*

result_of 推导类型

*/

double emulate(int n) {
	return 0;
}

void TestRest() 
{
	typedef double (*F)(int n);		//函数指针
	F f = emulate;
	boost::result_of<F(int)>::type x =f(45) ;
	std::cout<<" "<<typeid(x).name() << std::endl;
}

template<typename T,typename T1>
typename boost::result_of<T(T1)>::type call_func(T t, T1 t1)
{
	return t(t1);
}

void TestRest2() 
{
	typedef double (*F)(int n);
	F f = emulate;
	BOOST_AUTO(x,call_func(f,6));
	std::cout<<typeid(x).name()<<std::endl;
}

/*

ref库，将对象作为引用类型传参

*/

void TestRef() 
{
	int x = 10122;
	boost::reference_wrapper<int>ref_t(x);
	std:cout <<ref_t<< std::endl;

	string s;
	boost::reference_wrapper<string>ref_s(s);
	*ref_s.get_pointer() = "asdslsdsjlsjd";		
	std::cout<<ref_s.get()<<std::endl;		//不能直接打印包装字符串的对象，使用get方法获取
}

//使用ref()、cref()函数，简化写法

void TestRef2() {
	int x = 2321213;
	BOOST_AUTO(ini,boost::ref(x));
	
}

/*

bind 绑定函数、参数

*/

int f(int a,int b) 
{
	return a+b;
}

typedef int (*pfunc)(int, int);		//定义函数指针
pfunc pf1 = f;

typedef std::vector<int>(*pfunc2) (int);


struct ms1 {
	void show()
	{
		return;
	}
};

void TestBind() 
{
	int x = 1, y = 2;
	cout<< bind(f, 0, 0)()<<endl;
	cout<< bind(f, 0, _1)(x)<<endl;
	cout<< bind(pf1, _1, _2)(x,y)<<endl;
	bind(&ms1::show,_1);	//绑定成员函数，第一个位置加上成员函数的指针
	
}

void TestFunction()
{
	boost::function<int(int,char)>func;

}

//int main() {
//	TestBind();
//}







