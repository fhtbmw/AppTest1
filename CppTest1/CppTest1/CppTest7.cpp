#include <iostream>
#include "pch.h"
#include <vector>
#include <list>
#include <array>
#include <forward_list>
#include<set>
#include <map>
#include <deque>
#include <assert.h>
#include<windows.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <unordered_map>

using namespace std;

void test1()
{
	list<int> l1{3,4,2,31,3123,43,2,34,54};
	int iArr[10]{2,3,4,213,21323};

	vector<int> vt1(l1.begin(), l1.end());	//一种容器初始化另一种容器

	vector<int> vt2(sizeof(iArr)/sizeof(iArr[0]), 0);
	std::copy(iArr, iArr+3, vt2.begin());
}

void test2()
{
	array<int, 3> arr1({4,2,4343,});	//两个template参数,第二个参数为容器元素数量,不可缺少
	
	array<int, 3> arr2{};		//初始化元素内容为0
	arr2.fill(0);	//容器的元素都赋值为相同值

	array<int, 0> arr3;
	
	array<char, 4> arr4;	//C 风格array
	strcpy_s(arr4.data(), arr4.size(), "sad");  //data() 返回容器地址
	printf("%s \n", arr4.data());

	arr1 = std::move(arr2);


	array<int, 2> arr5 = { 3,2 };
	arr5.back() = 0;
}
 
void test3()
{
	vector<int> vt1 {2,3,2,3,1,3,2};
	int Size = vt1.size();
	int Capacity = vt1.capacity();
	assert(Capacity >= Size);

	vt1.resize(23);
	vt1.assign(3, 0);
	
	int ret = vt1.at(1);

	auto it = remove(vt1.begin(), vt1.end(), 0);
}

template<typename T>
void shrinkCapacity(vector<T>&v)
{
	vector<T> tmp(v);
	v.swap(tmp);
}

void test4()
{
	vector<int> vtInt1;
	vtInt1.push_back(2);
	vtInt1.push_back(3);
	vtInt1.push_back(4);
	vtInt1.push_back(6);
	vtInt1.push_back(5);

	vtInt1.pop_back();
	vtInt1.pop_back();
	vtInt1.pop_back();

	cout << vtInt1.max_size() << endl;
	cout << vtInt1.capacity() << endl;
}

void test5()
{
	//forward_list<int> fwlist = {23,4,3,1};  //单向链表
	//fwlist.insert_after(fwlist.before_begin(), 2);  //头插，before_begin 返回第一个元素之前的迭代器
	//fwlist.insert_after(fwlist.end(), 0);		Runtime error,end() 

	typedef set<float, std::greater<float>>::iterator	iterator;

	set<float, std::greater<float>> set1{0,3,2,3,1,3,23};	//元素不重复，递减排列	
	set<float, std::greater<float>> set2{ 100,2};
	assert(set1 < set2);	//排序相同的容器才可比较，比较相同位的元素

	float fVal = 10.0;

	set<float, std::greater<float>>::iterator it = set1.lower_bound(fVal);	//返回容器里首个 < val 的位;
	set1.insert(it, fVal);

	pair<iterator, iterator> pPair = set1.equal_range(fVal);		//返回  <= val <= 的区间
	cout << *pPair.first <<endl;
	cout << *pPair.second <<endl;

}

void test6()
{
	typedef map<float, std::string, std::less<float>>::iterator  iterator;
	
	map<float, std::string, std::less<float>> map1{ {1,"d"},{2, "dd"}};		// 

	auto ret = map1.key_comp();			// 返回容器排序规则
	auto ret2 = map1.value_comp();
	
	iterator it = map1.lower_bound(3);
	//map1.insert(it, {3, "ddd"});
	map1.emplace_hint(it,3,"ddd");
	map1.insert({0,"dsad"});

	map<float, std::string, std::less<float>> map2;

	/*for_each(map2.end(), map2.end(),
		[](std::pair<float, string>& elem) {
			elem.first --;
		});*/
}


typedef struct _DEVICE_PROFILE_TYPE
{
	uint8_t		uID;
	uint8_t		uName[30];
	uint8_t		uKey[90][6];
	uint8_t		uGameModeSwitch;
	uint8_t		uReserved1[2];
}DEVICE_PROFILE_TYPE, * PDEVICE_PROFILE_TYPE;

void test7()
{
	DEVICE_PROFILE_TYPE	 m_vtProfile;
	ZeroMemory(&m_vtProfile, sizeof(m_vtProfile));
	m_vtProfile.uKey[0][4] = 0x99;
	m_vtProfile.uKey[0][2] = 0x99;
	m_vtProfile.uKey[0][3] = 0x99;
	m_vtProfile.uKey[0][1] = 0x99;

	std::vector<uint8_t> vtKeyData(m_vtProfile.uKey[0],
		m_vtProfile.uKey[0] + 6 );

}

class f 
{
public:
	virtual void func() {
		cout << "f" << endl;
	
	}
};

class s :public f 
{
public:
	virtual void func(int a)
	{
		cout<< "s" << endl;

	}

};


void test8()
{
	s s1;
	f f1 = s1;

	enum mode {a,b};

	mode m1;

	f1.func();
	
}

class RuntimeStringCmp {
public:

	enum  cmp_mode {normal, nocase};

private:
	const cmp_mode mode;

	static bool nocase_compare(char c1, char c2)
	{
		return toupper(c1) < toupper(c2);
	}

public:
	RuntimeStringCmp(cmp_mode m =normal): mode(m){}

	bool operator() (const string& s1, const string& s2) const 
	{
		if (mode == normal)
		{
			return s1 < s2;
		}
		else 
		{
			return lexicographical_compare(s1.begin(), s1.end(), 
										   s2.begin(), s2.end(), 
										   nocase_compare);
		}
	}

};

typedef std::map<string, string, RuntimeStringCmp> StrMap;

void fillAndPrint(StrMap& coll);

void fillAndPrint(StrMap& coll)
{
	coll["aca"] = "a";
	coll["bab"] = "b";
	coll["Cbc"] = "c";
	coll["cc"] = "c";

	cout.setf(ios::left, ios::adjustfield);
	for (const auto& elem : coll)
	{
		cout << setw(15) << elem.first << " "
			<< elem.second << endl;
	}
}

void test9()
{
	StrMap	coll1;
	fillAndPrint(coll1);

	RuntimeStringCmp ig(RuntimeStringCmp::nocase);
	StrMap	coll2(ig);
	fillAndPrint(coll2);

	std::unordered_map<int, char> umap;
	umap.insert({1,'a'});

}

void main()
{
	test4();
}
