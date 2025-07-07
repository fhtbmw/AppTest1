#include "pch.h"
#include "BoostTest8.h"
#include<iostream>

using namespace std;
using namespace boost::assign;

void TestForeach() 
{
		
	vector<int>v = list_of(2)(3);

	BOOST_FOREACH(int x,v)
	{
		cout << x << " ";
	}
	
}

//void TestMinmax() 
//{
//	string s1("dasdasd"), s2("DADsA");
//	BOOST_AUTO(x,boost::minmax(s1,s2));
//	cout<<x.get<0>()<<endl;
//}


//int main() 
//{
//	TestMinmax();
//	return 0;
//}