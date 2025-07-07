#include "pch.h"

#include <string>
#include <iostream>
#include <vector>

using namespace std;

void test1()
{
	string sItem1 = "a.txt";

	string::size_type pos1 = sItem1.find(".");
	if (pos1 != string::npos)
	{
		string sSub2 = sItem1.substr(pos1, 5);
		string sSub3 = sItem1.substr(pos1, string::npos);	//npos 表明返回剩余所有字符
	}

	string sSub1 = sItem1.substr(1);		//返回指定位置之后的字符串
	
	string::size_type pos2 = sItem1.find_first_not_of("a");

	string::size_type pos3 = sItem1.find_first_of("x", 3);	//返回指定位置开始第一个指定string的位置
}

struct S1
{
	int a;
};

struct S2
{
	int a;
	char b;
};

void f1(S2* s)
{

}

void test2()
{
	//S1 s1;
	//S2* s2 = (S2*)&s1;		//较大结构体可初始化较小结构体对象

	//f1(s2);
	//int a[2];

	S2 s1, s2;
	s1.a = 0;
	s2.a = 0;

	if (memcmp(&s1, &s2, sizeof(S2)))
	{	
		cout << "-1" << endl;
	}
}

void test3()
{
	string s1 = "Masada's dassdassad  ";

	const char* ch1 = s1.c_str();

	cout<<s1.size()<<endl;
	cout<<s1.length()<<endl;

	const char * ch2 = s1.data();

	//cout << s1.max_size() << endl;

	char ch3 = s1.back();

	char* ch4 = &s1[2];

	char& ch5 = s1[4];

	cout << sizeof(ch1) << endl;	//指针大小

	cout << "s1 compare to ch4: " << s1.compare(ch4) << endl;
}

void test4()
{
	// string 赋值
	string s1 = "Dsa";

	string s2;

	s2.assign(s1);	//将参数赋值给调用者

	cout << s1 << s2 << endl;

	//string 清空
	s2.erase();
	s1.clear();

	//string 安插移除
	s1 += "end";
	s2 += {'n', 'k'};

	s2.append(s1, 1, 2);
	s2.push_back('i');
	s2.insert(2, "d");

	s2.pop_back();
	s2.erase(3);

 }

void test5()
{
	string s1 = "hand";

	size_t pos = s1.find("a");


}

void test6()
{
	std::vector<int> vtNum1{2,3,4};

	vtNum1 = { 0,0,0 };

	auto pos = vtNum1.at(2);


	vtNum1.emplace(vtNum1.begin(), 0);

	std::fill(vtNum1.begin(), vtNum1.end(), 0);

}

void test7()
{
	string s1 = "like";

	const char* ch1 = "dsadasad";

	cout << "s1 compare to ch1: " << s1.compare(ch1) << endl;

	char ch2 = s1.at(0);

	string s2;

	s2.swap(s1);

	s2.append("Dsaada");

	s2 += "dsada";

	s2.insert(s2.end(), 's');
	s2.insert(s2.end(), { 's' });

	s2.resize(25, '0');	//字符串长度增加，则以0填充增加的值

}

void test8()
{
	string s1 = "conference";
	string s2;

	s2.assign(s1);

	s1.replace(s1.begin(), s1.end(), s1.size(), 'c');
	 
	auto pos = s2.find('e');
	auto pos2 = s2.rfind('e');

	string sSub = s2.substr(0, 5);

	string s3 = "23";

	auto ret1 = stoi(s3);

}

int main()
{
	test8();
}
