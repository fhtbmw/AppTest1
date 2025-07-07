#include "pch.h"

#include <string>
#include <map>

using namespace std;

// 容器内的成员

void test1()
{
	string sTemp = "dsad";
	const char* p =sTemp.data();

	map<int, char> mTemp;
	mTemp.emplace(make_pair(1,'a'));

}






void main()
{

}