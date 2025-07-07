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
	//transform ������������ظ���ֵԪ��
	vector<int>vtT1 = { 2,3,45,5 };
	transform(vtT1.begin(), vtT1.end(), vtT1.begin(), calc2);

	std::random_device rd;
	std::mt19937 rng(rd());

	shuffle(vtT1.begin(), vtT1.end(), rng);	//����Ԫ��˳��

	for (auto item : vtT1)
	{
		item++;
	}


}

// �����㷨  
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

	int index = count(vtNum1.begin(), vtNum1.end(), 3);	//count �����������ض�Ԫ�صĳ��ִ���

	int index2 = count_if(vtNum1.begin(), vtNum1.end(),	//count_if ����������ʹ�ú���Ϊtrue��Ԫ�ش���
		[](int item) 
		{
			return item % 2 == 0;
		});

	cout << index2 << endl;

	auto data = minmax_element(vtNum1.begin(), vtNum1.end());	//�������������Сֵ, pair
	auto mindata = *(data.first);
	auto maxdata = *(data.second);

	vector<int>::iterator it = find(vtNum1.begin(), vtNum1.end(), 0); //���������е�ָ��ֵ

	vector<int>::iterator it2 = find_if(vtNum1.begin(), vtNum1.end(),	//���������е�һ��ʹ�ú���Ϊtrue��ֵ		
		[](int item)
		{
			return item % 2 != 0;
		});
	
	vector<int>::iterator it3 = find_if_not(vtNum1.begin(), vtNum1.end(),	//���������е�һ��ʹ�ú���Ϊfalse��ֵ
		[](int item)
		{
			return item < 0;
		});

	cout << *it3 << endl;
}

void test7()
{
	vector<int>vtNum{2,43,5,34,34,1};	

	auto pos1 = search_n(vtNum.begin(), vtNum.end(), 2, 34); //��������2��Ԫ��ֵ��ͬ�ĵ�һ��Ԫ�ص�λ��
	auto pos2 =	search_n(vtNum.begin(), vtNum.end(), 1,	5,	//��������1��Ԫ��ֵ���㺯���ĵ�һ��Ԫ��λ��
		[](int item, int value)
		{
			return item % 5 == 0;
		});

	list<int>lNum{34,34};

	vector<int>::iterator pos3 = find_end(vtNum.begin(), vtNum.end(), lNum.begin(), lNum.end());	//����������ָ�����е��������ĵ�һ��Ԫ��λ��
	
	auto dis = std::distance(vtNum.begin(), pos3);

	/*  �� lambda �в������ʱ������ķ�ʽ��Ӱ����������ͣ�
		ֵ����[=] ������ᴴ��һ����������ĸ���������������һ������������ vtNum.begin()������ô�� lambda �ڲ�ʹ�õĽ�������������ĸ�����
		���ò���[&]������Ჶ����������ã������� lambda �ڲ�ֱ��ʹ���ⲿ������*/

	[=]() 
		{
			//auto dis = std::distance(vtNum.begin(), pos3);	// ʹ��ֵ���� vtNum.begin()��Ϊ���������� 
		};

	[&]()
		{
			auto dis = std::distance(vtNum.begin(), pos3);		//���ò���,ֱ�ӻ�ȡ�ⲿ��������������
		};

}

void test8()
{
	vector<int>vtNum{0,4,1,3,3,1,2,2};

	list<int>lNum{ 0,4,1,3,6,5,6,4,4,5};

	auto pos1 = find_first_of(vtNum.begin(), vtNum.end(), lNum.begin(), lNum.end());  //find_first_of ����������ָ�����еĵ�һ��Ԫ��λ��

	auto pos2 = adjacent_find(vtNum.begin(), vtNum.end());	//adjacent_find���������������������Ԫ�صĵ�һ��Ԫ��λ��

	auto pos3 = adjacent_find(vtNum.begin(), vtNum.end(),	//�������������ʹ�ú������Ϊtrue�ĵ�һ��Ԫ��λ��
		[](int item1, int item2)
		{
			return item1 % 2 == 0 && item2 % 2 == 0 && item1 >0;
		});

	bool pos4 = equal(vtNum.begin(), vtNum.end(), lNum.begin());	//�Ƚ����������Ƿ����,�ڶ�����Ԫ�ظ���>=��һ������Ԫ�ظ���

}

void test9()
{
	vector<int>vtNum1{4,65,21,4,2,5,43};
	vector<int>vtNum2{4,65,21,2,4,5,54,34,4,3};		

 	bool ret = is_permutation(vtNum1.begin(), vtNum1.end(), vtNum2.begin());	//��һ�������Ƿ��ǵڶ���������������,�ڶ�����Ԫ�ظ������ڵ�һ������Ԫ�ظ���

	auto pos1 = mismatch(vtNum1.begin(), vtNum1.end(), vtNum2.begin());		//���ص�һ�� ��һ��������ڶ����䲻ͬ������Ԫ��

	bool ret2 = lexicographical_compare(vtNum1.begin(), vtNum1.end(), vtNum2.begin(), vtNum2.end());	//��һ������Ԫ���Ƿ��ֵ���С�ڵڶ������Ԫ��								
}

void test10()
{
	vector<int>vtN{2,3,4,5};
	vtN.insert(vtN.begin() + 0x02, 0);

	auto ret = is_sorted(vtN.begin(), vtN.end());	//���������Ԫ���Ƿ�����

	bool ret2 = is_partitioned(vtN.begin(), vtN.end(), 		//�ж�����Ԫ���Ƿ񱻷ָ�,���Ϻ�����Ԫ����ǰ
		[](int item)
		{
			return item % 2 == 0;
		});

	bool ret3 = is_heap(vtN.begin(), vtN.end());	//�ж�����Ԫ���Ƿ��γɶѣ������ֵ��ǰ

	bool ret_all = all_of(vtN.begin(), vtN.end(),	//�ж�����Ԫ���Ƿ�ȫ����������
		[](int item)
		{
			return item > 0;
		});

	bool ret_any = any_of(vtN.begin(), vtN.end(),	//�ж������Ƿ���Ԫ����������
		[](int item)
		{
			return item > 0;
		});

	bool ret_none = none_of(vtN.begin(), vtN.end(),		//�ж�����Ԫ���Ƿ񶼲���������
		[](int item)
		{
			return item > 0;
		});
 }



void test11()
{
	std::vector<int> vtSource{1,3,3,4};
	std::vector<int> vtDestian(4);

	std::copy(vtSource.begin(), vtSource.end(), vtDestian.begin());	//����Ԫ�ظ��Ƶ��ڶ������䣬�ڶ�����Ԫ�������ڵ�һ����

	std::copy_if(vtSource.begin(), vtSource.end(), vtDestian.begin(),	//��������������Ԫ��
		[](int item)
		{
			return item % 2 == 0;
		});

	std::copy_backward(vtSource.begin(), vtSource.end(), vtDestian.end());	//������

}

void test12()
{
	std::vector<int> vtNum1{1,3,5,7,9};
	std::vector<int> vtNum2{ 2,4,6,8,10 };

	std::transform(vtNum1.begin(), vtNum1.end(), vtNum2.begin(),	//����һ����Ԫ�ؽ��в����������ڶ�������
		[](int item)
		{
			return ++item;
			return ++item;
		});

	fill_n(back_inserter(vtNum2), 10, 0);	//���Ԫ�أ� back_inster ��������ĩβ�ĵ�����
	
	generate(vtNum2.begin(), vtNum2.end(),	//������������ֵ����ָ������
		[]()
		{
			return 1;
		});

	std::list<int> lNum3{0,0,0,0,0};
	std::iota(lNum3.begin(), lNum3.end(), 0);	//�����丳���������

	replace(lNum3.begin(), lNum3.end(), 0, -1);

}

void test13()
{
	std::vector<double> vtNum1{-2,-1,0,1,2};
	auto pos = remove(vtNum1.begin(), vtNum1.end(), 0);	//֮���Ԫ�ػ���ǰ�ƶ������Ǳ��Ƴ�Ԫ��		
														//remove ������������յ�
	vtNum1.erase(pos);

	std::list<int> lNum2{ 0,0,1,1,2,2 };
	auto pos1 = unique(lNum2.begin(), lNum2.end());	//unique ����ָ�����߼������յ�ĵ�����				

	for (auto i = lNum2.begin(); i != pos1; i++)
	{
		cout << *i << endl;
	}

	reverse(lNum2.begin(), lNum2.end());	//��ת
	rotate(vtNum1.begin(), vtNum1.begin()+2, vtNum1.end());	//��ת,�Դ���ĵڶ���������Ϊ�����
	
	partition(vtNum1.begin(), vtNum1.end(),		//�����㺯��������Ԫ��ǰ��
		[](int item)							//���ص�һ��������������Ԫ�صĵ�����
		{
			return item % 2 == 0;
		});
}

void test14()
{
	deque<int>dqNum1{ 2,34,40,4 };

	list<int>lNum2{ 0,10,20304,2 };

	sort(dqNum1.begin(), dqNum1.end());
	//sort(lNum2.begin(), lNum2.end());		//ֻ��֧��������ʴ�ȡ������������

	sort(dqNum1.begin(), dqNum1.end(),
		[](int item, int item2)
		{
			return item > item2;
		});

	partial_sort(dqNum1.begin(), dqNum1.end(), dqNum1.end());	//�������Ĳ���Ԫ�ؽ�������
}

void test15()	//Heap�㷨
{
	std::vector<int> vtnum1{ 3,4,5,6 };
	std::vector<int> vtnum2{ 0,1,2,7 };

	make_heap(vtnum1.begin(), vtnum1.end());	//ת����heap
	vtnum1.push_back(9);

	push_heap(vtnum1.begin(), vtnum1.end());	//ά���ѵ����ݽṹ
	
	pop_heap(vtnum1.begin(), vtnum1.end());		//���Ԫ���Ƶ��������Ԫ�ؼ���ά�ֶѽṹ
}

void test16()	//�����������Ԫ���㷨
{
	std::vector<int> vtNum1{ 0,203,4,3,5,8 };

	sort(vtNum1.begin(), vtNum1.end());

	bool ret = binary_search(vtNum1.begin(), vtNum1.end(), 8);	

	auto pos1 = lower_bound(vtNum1.begin(), vtNum1.end(), 4);	//���ص�һ����С��ָ��ֵ��Ԫ�ص�λ��
	cout << *pos1;

	auto pos2 = equal_range(vtNum1.begin(), vtNum1.end(), 3);	//���غ�ָ��ֵ��ȵ�����

}

void test17()	//�ϲ�Ԫ��
{
	std::list<int>lNum1{1,3,5,7,9};		
	std::list<int>lNum2{ 2,4,6,8 };

	//merge �ϲ����������������
	merge(lNum1.begin(), lNum1.end(), lNum2.begin(), lNum2.end(), ostream_iterator<int>(cout, ""));		
	cout << endl;
	
	set_difference(lNum1.begin(), lNum1.end(), lNum2.begin(), lNum2.end(),
		ostream_iterator<int>(cout, ""));
	cout << endl;
	
	set_intersection(lNum1.begin(), lNum1.end(), lNum2.begin(), lNum2.end(),
		ostream_iterator<int>(cout, ""));
	cout << endl;

	std::vector<int> vtNum3{ 2,3,4,1,7,8 };
	
	vector<int>::iterator it = find(vtNum3.begin(), vtNum3.end(), 4);	//����ָ��ֵ��ǰһ������
	sort(vtNum3.begin(), it);
	sort(it, vtNum3.end());

	inplace_merge(vtNum3.begin(), it, vtNum3.end());

}

void test18()
{
	std::vector<int> vtNum1{ 30,4,3,5,5,03,4,7 };
	int ret = accumulate(vtNum1.begin(), vtNum1.end(), 1);	//��ָ��ֵ��ʼ������Ԫ�ؽ����ۼ�
	partial_sum(vtNum1.begin(), vtNum1.end(), ostream_iterator<int>(cout, " "));
}


void main()
{
	//test5();
	//unsigned short s1 = 553434234324242;
	//cout << s1 << endl;
	
   	test18();
}

