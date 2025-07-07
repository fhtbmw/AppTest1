#include "pch.h"
#include <vector>
#include <list>
#include <forward_list>
#include<set>
#include <iostream>
#include <algorithm>
#include <assert.h>
#include <deque>

using namespace std;

void Test1()
{
	int arr[] = {2,3,3};
	vector<int> vt1(arr, arr+2);
	vt1.push_back(2);
	vt1.push_back(2);
	vt1.push_back(2);
	vt1.push_back(2);
	vt1.push_back(2);
	vt1.push_back(2);
	vt1.push_back(1);
	vt1.push_back(3);



	auto minelement =min_element(vt1.begin(), vt1.end());
	
	reverse(find(vt1.begin(),vt1.end(),2), vt1.end());

	vector<int>::iterator maxelement = find(vt1.begin(), vt1.end(), 3);

	vector<int> vt2 = {2,3,5,1,3,1,3,4};

	bool flag = equal(vt1.begin(), vt1.end(), vt2.begin());
	copy(vt1.begin(), vt1.end(), vt2.begin());		//equal、copy,传入的参数其空间大小要一致
}

void Test2()
{
////	list<WORD> L1;	//list 双向链表
//	L1.push_back('a');
//	cout<<L1.front();
//	L1.pop_front();
//
//	forward_list<BYTE> fl1;	//单向链表
}

void Test3()
{
	list<char> L1;
	for (list<char>::iterator pos = L1.begin(); pos != L1.end(); ++pos)
	{
	}

	//常量迭代器,只可以读元素，不可修改
	for (list<char>::const_iterator pos = L1.begin(); pos != L1.end(); ++pos)
	{
	}

	set<int> S1;		//元素递增排序
	set<int, greater<int>> S2;		//元素递减排序
	S1.insert({ 2,3,4,4,1,1 });
	S2.insert({1,4,5,6,7,1});

	vector<int> vtv = {3,4,5,1,3,6,56};
	copy(vtv.crbegin(), vtv.crend(), ostream_iterator<int>(cout, ""));  //反向迭代器
	cout << endl;
}

void Test4()
{
	list<int> coll;

	for (int i=1 ;i<6; ++i) 
	{
		coll.push_back(i);
		coll.push_front(i);
	}

	copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, ""));
	cout << endl;
																				
	list<int>::iterator end =remove(coll.begin(), coll.end(),1);  //remove元素,未删除元素，将元素间顺序改变 获取list的新的逻辑终点

	vector<int> vtV1 = {2,4,5,443,2};
	remove(vtV1.begin(), vtV1.end(), 2);

	//copy(coll.cbegin(), coll.cend(), ostream_iterator<int>(cout, ""));
	copy(coll.begin(), end, ostream_iterator<int>(cout, ""));		
	cout << endl;

	cout<<" number of removed elements:" << distance(end,coll.end()) <<endl;

	coll.erase(end, coll.end());			//erase 真正删除元素

}

void Test5()
{
	set<int> Set = {2,3,3,4,4,43};
	
	Set.erase(3);

	copy(Set.begin(), Set.end(), ostream_iterator<int>(cout, ""));
	cout << endl;
}

//struct keyBoard
//{
//	 const uint8_t	pid ;
//	 const char name;
//
//	 keyBoard(uint8_t n, char m):pid(n), name(m){}
//};
//
//bool SortFunc(const keyBoard& kb1, const keyBoard& kb2)
//{
//	return kb1.pid < kb2.pid || 
//		(kb1.pid == kb2.pid &&
//		 kb1.name <= kb2.name);
//}

void Test6()
{
	/*deque<keyBoard> dqQueue;
	keyBoard k1{1,'v'};
	keyBoard k2{2,'/'};
	keyBoard k3{3,'a'};

	dqQueue.push_back(k1);
	dqQueue.push_back(k2);
	dqQueue.push_back(k3);

	sort(dqQueue.begin(), dqQueue.end(), SortFunc);
	
	for (deque<keyBoard>::const_iterator it = dqQueue.begin(); it != dqQueue.end(); ++it)
	{
		cout << it->name <<endl;
	}*/

}

void Test
()
{
	deque<int> dqColl = { 1,4,4,2,5,2,3,43,0};


}

void Test7()
{
	int i = 4;

	if (i <5)
	{
		cout << "< 5" << endl;
	}
	if (i<6)
	{
		cout << "<6" << endl;

	}

}

void main()
{
  	//Test5();
	/*typedef unsigned short  in;
	typedef unsigned char   ch;
	cout<<sizeof(ch);*/

	Test7();

}






