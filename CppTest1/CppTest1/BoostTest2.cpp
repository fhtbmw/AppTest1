#include "pch.h"
#include<boost/make_shared.hpp>
#include<boost/smart_ptr.hpp>
#include<boost/enable_shared_from_this.hpp>
#include<iostream>
#include<string.h>
#include<vector>

using namespace std;
using namespace boost;


void MakeSharedTest() {
	boost::shared_ptr<int>sp1 = boost::make_shared<int>(1);
	boost::shared_ptr<std::string>sp2 = boost::make_shared<std::string>("test");
	boost::shared_ptr<std::vector<char>>sp3 = boost::make_shared<std::vector<char>>(10,'a');
	std::cout <<*sp2<<std::endl;
}

void MakeSharedTest2() {
	typedef std::vector<boost::shared_ptr<int>> vv;
	vv v1(10);
	
	int num = 0;
	for (vv::iterator it = v1.begin(); it != v1.end();++it) {
		(*it) = boost::make_shared<int>(num++);
		std::cout << *(*it)<<std::endl;
	}

}

void TestWeakPtr() {
	boost::shared_ptr<int>sp(new int(20));
	boost::weak_ptr<int>wp(sp);
	boost::weak_ptr<int>wp2(wp);
	assert(sp.use_count() == 1);
	sp.reset();		//重置shared_ptr，失效引用计数为0
	cout << wp.use_count();
}

//继承enable_shared_from_this,获取类this指针的shared ptr
class self:public boost::enable_shared_from_this<self> {	
public:
	self(int n,char c):x(n),name(c) {
	}
	void print() {
		cout<<(this)<<"self: "<<x << endl;
	}
	int x;
	char name;
	
};

void test_self() {
	boost::shared_ptr<self>sp1 = boost::make_shared<self>(2, 'c');
	boost::shared_ptr<self>sp3(new self(3,'d'));
	boost::shared_ptr<self>sp2 = sp1->shared_from_this();	//只可从shared_ptr获取，如不是会在运行时出错
	sp1->print();
	sp2->x = 1;
	sp2->print();
	sp1->print();
	assert(sp3.use_count()==1);
	cout << sp2.use_count();
}

//shared_ptr<int> Add(void) {
//	int a = 4;
//	shared_ptr<int>sp(&a);
//	return sp;
//}

void main()
{
	//TestWeakPtr();
	test_self();
}

