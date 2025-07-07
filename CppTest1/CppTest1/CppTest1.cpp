#include"BoostTest6.h"
#include<iostream>
using namespace std;

struct ss {
	int age;
	char flag;
	double id;
};

void t1() {
	ss* s1 = new ss();
	ss s2 = ss();
	cout << sizeof(s2) << " " << sizeof(*s1) << endl;
	int* p = new int[10];
	cout << sizeof(*p) << endl;
}

double* _getchar() {
	double *p =(double*)malloc(sizeof(double)*10000000);
	*p;
	return p;
}



void t2() {
	/*double* cp = _getchar();
	cout << *cp;*/
	
}

struct F {
	F(int x):name("") {
		
	}
	string name;
};

struct S :public F{
	S(): name(""),F(2){			//父类构造函数有参数，子类初始化列表需写入
		
	}

	S& operator=(const S&in) {
		if(this==&in){
			return *this;
		}
		this->name = in.name;
		return *this;
	}

	void t(const string &in) {
		if (this->name==in) {
			cout<<"Same" << endl;
			return;
		}
		name = in;
	}
	string name;
};

void t3() {
	//S s1;
	//g_mal = 2;
}

/*

Has-A 和 Is-A
组合 -- 继承 

*/

/*

组合：类之间有包含关系，则小类对象作为大类的成员

*/
class Hand {
public:
	void Hit() {}
};

class Foot{
public:
	void Walk() {}
};

class Mouth {
public:
	void Speek() {}
};

class Human {
public:
	Human():name("") {
	}

	void Modify(int& num) {
		if (num) {
			this->id = num;
		}
		num = 1;
	}

	void Hit(){
		h.Hit();
	}

	Human& operator=(const Human& in) {
		if (this==&in) {
			return *this;
		}

		this->f = in.f;
		this->h = in.h;
		this->m = in.m;
		this->id = in.id;

		cout<<"this "<<this <<" in:"<< &in << endl;

		return *this;
	}

private:
	Hand h;
	Foot f;
	Mouth m;
	string name;
	int id;
};

int g_mal;

void t4() {
	Human hu,hu2,hu3;
	(hu = hu2) = hu3;
//	hu.Modify(g_mal);
}

//int main() {
//	//t2();
//	//t3();
//	//t4();
//	//cout << g_mal;
//	
//	return 0;
//}