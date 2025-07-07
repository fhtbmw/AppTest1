#include "pch.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <cassert>
#include <list>
#include <numeric>
#include <deque>

using namespace std;

/*

STL algorithm exercise

*/

template<typename T>
void calc(T t)
{
	return t + 1;
}

int calc2(int t)
{
	return t + 1;
}

void test1()
{
	//transform 将操作结果返回给赋值元素
	vector<int>vtT1 = { 2,3,45,5 };
	transform(vtT1.begin(), vtT1.end(), vtT1.begin(), calc2);

	std::random_device rd;
	std::mt19937 rng(rd());

	shuffle(vtT1.begin(), vtT1.end(), rng);	//打乱元素顺序

	for (auto item : vtT1)
	{
		item++;
	}


}

// 排序算法  
	/*

	sort
	stable_sort
	partial_sort
	nth_element

	*/

void test2()
{
	vector<int> vtV1(4);
	vector<int> vtV2(4);

	int data[] = {2,3,4};
	memcpy(&vtV1[1],&data[0], 3);

	memcpy(&vtV2[0],&vtV1[0], 3);


}

struct T
{
	unsigned char a1;
};

typedef struct _D
{
	T t;
	unsigned char a;
	unsigned char b;

}D, *PD;

void test3()
{
	/*unsigned char i = 2;
	std::vector<unsigned char>vtV1{1,i};*/
	T t1{0};
	

	D d1{t1,1,2};

	PD p1 = &d1;
	/*p1->t.a1 - 0;
	p1->a = 1;*/

	std::vector<unsigned char>vtV2(sizeof(D));

	memcpy(vtV2.data(), &p1->t, sizeof(D));

}

void test4()
{
	while (1)
	{
		int n, m;
		cin >> n >> m;
		double ret = n + m;
		cout << n << "+" << m << " =" << ret << endl;
	}
}

void test5()
{
	while (1)
	{
		double w, cc;
		cin >> w >> cc;
		
		double ret = w * cc;
		cout<< w <<"*" <<cc <<"="<<ret << endl;

	}

}

void test6()
{
	vector<int> vtNum1{2,7,3,5,34,4,3};

	int index = count(vtNum1.begin(), vtNum1.end(), 3);	//count 计算容器中特定元素的出现次数

	int index2 = count_if(vtNum1.begin(), vtNum1.end(),	//count_if 计算容器中使得函数为true的元素次数
		[](int item) 
		{
			return item % 2 == 0;
		});

	cout << index2 << endl;

	auto data = minmax_element(vtNum1.begin(), vtNum1.end());	//返回容器最大最小值, pair
	auto mindata = *(data.first);
	auto maxdata = *(data.second);

	vector<int>::iterator it = find(vtNum1.begin(), vtNum1.end(), 0); //返回容器中的指定值

	vector<int>::iterator it2 = find_if(vtNum1.begin(), vtNum1.end(),	//返回容器中第一个使得函数为true的值		
		[](int item)
		{
			return item % 2 != 0;
		});
	
	vector<int>::iterator it3 = find_if_not(vtNum1.begin(), vtNum1.end(),	//返回容器中第一个使得函数为false的值
		[](int item)
		{
			return item < 0;
		});

	cout << *it3 << endl;
}

void test7()
{
	vector<int>vtNum{2,43,5,34,34,1};	

	auto pos1 = search_n(vtNum.begin(), vtNum.end(), 2, 34); //返回连续2个元素值相同的第一个元素的位置
	auto pos2 =	search_n(vtNum.begin(), vtNum.end(), 1,	5,	//返回连续1个元素值满足函数的第一个元素位置
		[](int item, int value)
		{
			return item % 5 == 0;
		});

	list<int>lNum{34,34};

	vector<int>::iterator pos3 = find_end(vtNum.begin(), vtNum.end(), lNum.begin(), lNum.end());	//返回容器中指定序列的最后区间的第一个元素位置
	
	auto dis = std::distance(vtNum.begin(), pos3);

	/*  在 lambda 中捕获变量时，捕获的方式会影响变量的类型：
		值捕获（[=] ）：这会创建一个捕获变量的副本。如果捕获的是一个迭代器（如 vtNum.begin()），那么在 lambda 内部使用的将是这个迭代器的副本。
		引用捕获（[&]）：这会捕获变量的引用，允许在 lambda 内部直接使用外部变量。*/

	[=]() 
		{
			//auto dis = std::distance(vtNum.begin(), pos3);	// 使用值捕获 vtNum.begin()变为常量迭代器 
		};

	[&]()
		{
			auto dis = std::distance(vtNum.begin(), pos3);		//引用捕获,直接获取外部迭代器变量引用
		};

}

void test8()
{
	vector<int>vtNum{0,4,1,3,3,1,2,2};

	list<int>lNum{ 0,4,1,3,6,5,6,4,4,5};

	auto pos1 = find_first_of(vtNum.begin(), vtNum.end(), lNum.begin(), lNum.end());  //find_first_of 返回容器中指定序列的第一个元素位置

	auto pos2 = adjacent_find(vtNum.begin(), vtNum.end());	//adjacent_find返回容器中连续两个相等元素的第一个元素位置

	auto pos3 = adjacent_find(vtNum.begin(), vtNum.end(),	//返回连续相等且使得函数结果为true的第一个元素位置
		[](int item1, int item2)
		{
			return item1 % 2 == 0 && item2 % 2 == 0 && item1 >0;
		});

	bool pos4 = equal(vtNum.begin(), vtNum.end(), lNum.begin());	//比较两个区间是否相等,第二区间元素个数>=第一个区间元素个数

}

void test9()
{
	vector<int>vtNum1{4,65,21,4,2,5,43};
	vector<int>vtNum2{4,65,21,2,4,5,54,34,4,3};		

 	bool ret = is_permutation(vtNum1.begin(), vtNum1.end(), vtNum2.begin());	//第一个区间是否是第二个区间的排列组合,第二区间元素个数大于第一个区间元素个数

	auto pos1 = mismatch(vtNum1.begin(), vtNum1.end(), vtNum2.begin());		//返回第一处 第一区间中与第二区间不同的两个元素

	bool ret2 = lexicographical_compare(vtNum1.begin(), vtNum1.end(), vtNum2.begin(), vtNum2.end());	//第一区间内元素是否字典序小于第二区间的元素								
}

void test10()
{
	vector<int>vtN{2,3,4,5};
	vtN.insert(vtN.begin() + 0x02, 0);

	auto ret = is_sorted(vtN.begin(), vtN.end());	//检测区间内元素是否有序

	bool ret2 = is_partitioned(vtN.begin(), vtN.end(), 		//判断区间元素是否被分割,符合函数的元素在前
		[](int item)
		{
			return item % 2 == 0;
		});

	bool ret3 = is_heap(vtN.begin(), vtN.end());	//判断区间元素是否形成堆，即最大值在前

	bool ret_all = all_of(vtN.begin(), vtN.end(),	//判断区间元素是否全部满足条件
		[](int item)
		{
			return item > 0;
		});

	bool ret_any = any_of(vtN.begin(), vtN.end(),	//判断区间是否有元素满足条件
		[](int item)
		{
			return item > 0;
		});

	bool ret_none = none_of(vtN.begin(), vtN.end(),		//判断区间元素是否都不满足条件
		[](int item)
		{
			return item > 0;
		});
 }



void test11()
{
	std::vector<int> vtSource{1,3,3,4};
	std::vector<int> vtDestian(4);

	std::copy(vtSource.begin(), vtSource.end(), vtDestian.begin());	//区间元素复制到第二个区间，第二区间元素数大于第一区间

	std::copy_if(vtSource.begin(), vtSource.end(), vtDestian.begin(),	//复制满足条件的元素
		[](int item)
		{
			return item % 2 == 0;
		});

	std::copy_backward(vtSource.begin(), vtSource.end(), vtDestian.end());	//反向复制

}

void test12()
{
	std::vector<int> vtNum1{1,3,5,7,9};
	std::vector<int> vtNum2{ 2,4,6,8,10 };

	std::transform(vtNum1.begin(), vtNum1.end(), vtNum2.begin(),	//将第一区间元素进行操作，填充入第二个区间
		[](int item)
		{
			return ++item;
			return ++item;
		});

	fill_n(back_inserter(vtNum2), 10, 0);	//填充元素， back_inster 返回容器末尾的迭代器
	
	generate(vtNum2.begin(), vtNum2.end(),	//将函数产生的值赋给指定区间
		[]()
		{
			return 1;
		});

	std::list<int> lNum3{0,0,0,0,0};
	std::iota(lNum3.begin(), lNum3.end(), 0);	//给区间赋予递增序列

	replace(lNum3.begin(), lNum3.end(), 0, -1);

}

void test13()
{
	std::vector<double> vtNum1{-2,-1,0,1,2};
	auto pos = remove(vtNum1.begin(), vtNum1.end(), 0);	//之后的元素会向前移动，覆盖被移除元素		
														//remove 返回新区间的终点
	vtNum1.erase(pos);

	std::list<int> lNum2{ 0,0,1,1,2,2 };
	auto pos1 = unique(lNum2.begin(), lNum2.end());	//unique 返回指向新逻辑区间终点的迭代器				

	for (auto i = lNum2.begin(); i != pos1; i++)
	{
		cout << *i << endl;
	}

	reverse(lNum2.begin(), lNum2.end());	//反转
	rotate(vtNum1.begin(), vtNum1.begin()+2, vtNum1.end());	//旋转,以传入的第二个迭代器为新起点
	
	partition(vtNum1.begin(), vtNum1.end(),		//将满足函数条件的元素前移
		[](int item)							//返回第一个不满足条件的元素的迭代器
		{
			return item % 2 == 0;
		});
}

void test14()
{
	deque<int>dqNum1{ 2,34,40,4 };

	list<int>lNum2{ 0,10,20304,2 };

	sort(dqNum1.begin(), dqNum1.end());
	//sort(lNum2.begin(), lNum2.end());		//只有支持随机访问存取的容器可排序

	sort(dqNum1.begin(), dqNum1.end(),
		[](int item, int item2)
		{
			return item > item2;
		});

	partial_sort(dqNum1.begin(), dqNum1.end(), dqNum1.end());	//针对区间的部分元素进行排序
}

void test15()	//Heap算法
{
	std::vector<int> vtnum1{ 3,4,5,6 };
	std::vector<int> vtnum2{ 0,1,2,7 };

	make_heap(vtnum1.begin(), vtnum1.end());	//转换成heap
	vtnum1.push_back(9);

	push_heap(vtnum1.begin(), vtnum1.end());	//维护堆的数据结构
	
	pop_heap(vtnum1.begin(), vtnum1.end());		//将最顶元素移到最后，其余元素继续维持堆结构
}

void test16()	//有序区间查找元素算法
{
	std::vector<int> vtNum1{ 0,203,4,3,5,8 };

	sort(vtNum1.begin(), vtNum1.end());

	bool ret = binary_search(vtNum1.begin(), vtNum1.end(), 8);	

	auto pos1 = lower_bound(vtNum1.begin(), vtNum1.end(), 4);	//返回第一个不小于指定值的元素的位置
	cout << *pos1;

	auto pos2 = equal_range(vtNum1.begin(), vtNum1.end(), 3);	//返回和指定值相等的区间

}

void test17()	//合并元素
{
	std::list<int>lNum1{1,3,5,7,9};		
	std::list<int>lNum2{ 2,4,6,8 };

	//merge 合并的区间需是有序的
	merge(lNum1.begin(), lNum1.end(), lNum2.begin(), lNum2.end(), ostream_iterator<int>(cout, ""));		
	cout << endl;
	
	set_difference(lNum1.begin(), lNum1.end(), lNum2.begin(), lNum2.end(),
		ostream_iterator<int>(cout, ""));
	cout << endl;
	
	set_intersection(lNum1.begin(), lNum1.end(), lNum2.begin(), lNum2.end(),
		ostream_iterator<int>(cout, ""));
	cout << endl;

	std::vector<int> vtNum3{ 2,3,4,1,7,8 };
	
	vector<int>::iterator it = find(vtNum3.begin(), vtNum3.end(), 4);	//返回指定值的前一迭代器
	sort(vtNum3.begin(), it);
	sort(it, vtNum3.end());

	inplace_merge(vtNum3.begin(), it, vtNum3.end());

}

void test18()
{
	std::vector<int> vtNum1{ 30,4,3,5,5,03,4,7 };
	int ret = accumulate(vtNum1.begin(), vtNum1.end(), 1);	//从指定值开始将区间元素进行累加
	partial_sum(vtNum1.begin(), vtNum1.end(), ostream_iterator<int>(cout, " "));
}


void main()
{
	//test5();
	//unsigned short s1 = 553434234324242;
	//cout << s1 << endl;
	
   	test18();
}

