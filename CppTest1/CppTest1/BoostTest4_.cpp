#include<boost/noncopyable.hpp>
#include <iostream>
#include<boost/optional.hpp>
#include<boost/typeof/typeof.hpp>
#include<vector>
#include<boost/assign.hpp>
#include<map>
#include<ctime>

typedef boost::noncopyable noncopyable;

using namespace std;
using namespace boost;

/*
noncopyable�ཫ�������캯����operator= ��Ϊ˽������,����̳к��ֹ�˿�������ֵ��Ϊ
*/
class Test :noncopyable {

};

void Test1(){
	Test t1;
	Test t2;
	//Test t3 (t1);		//����
}

/*

optional,�ɵ�����һ������ָ��,���ù㷺 

*/

void OptionalTest() {
	optional<int>op1;
	vector<int>v1(10);
	optional<vector<int>&>op2(v1);
	op2->push_back(2);
	for (auto it = op2->begin(); it != op2->end();it++) {
		//cout<<*it<<" ";
	}

	BOOST_AUTO(n,make_optional(2));
}

//��Ϊ��������ֵ
optional<int>compute(int x) {
	//int t = x - 1;
	return optional<int>(x>0,x-1);
}

/*

assign�⣬���Լ��׵ض�STL�е��������в���

*/

/*

assign �ṩ�������� push_back() push_front insert(),���ǿ�������ӵ��ͬ������������
map_list_of  pair_list_of  list_of()���ڹ�������ʱ�����������

*/

void AssignTest() {
	using namespace boost::assign;
	vector<char>cv;
	
	cv += '2', '23', 'dasd';
	map<int, string>map1;
	map1 += make_pair(1, "a"), make_pair(2, "b");	//map������Ҫmake_pair������Ԫ��
	push_back(cv)(2)(3)(1223213113)('asds');		//ʹ��assign���µĺ���
	insert(map1)(3,"c")(4,"d");				//ʹ��assign�⺯������д��

	set<string>ss=list_of("dasd")("dsadad")("dsdsd");
	//set<string>sss=(list_of("dasd"),"dsadad","dsdsd");  //��϶���д��
	
	map<int,string>mi = map_list_of(1,"aa")(2,"bb");

}

/* 

֧������������ 

*/

/*

����������û��inert��push_back�ȱ�׼�����ĳ�Ա������assignʹ��ʱ���ڳ�ʼ��ʱ������ݣ�
�������.to_adapter()�������䵽������������

*/

void AssignTest2()
{
	using namespace boost::assign;
	stack<int> si = list_of(3)(3)(4).to_adapter();
	while (!si.empty())
	{
		cout<<si.top()<<endl;
		si.pop();
	}
}

void PrintTime() {
	time_t now = time(nullptr);
	char time[26];
	ctime_s(time,sizeof(time), &now);
	cout<<"********"<<time<<"********" << endl;
}

/*

boost ���е�swap����;
std ���е�swap���������Զ���������˵��ִ��Ч�ʵ�(�ڲ����п������졢��ֵ�Ȳ���)

*/

void SwapTest() {
	int n1[10];
	int n2[10];
	fill_n(n1,10,1);
	fill_n(n2,10,2);
	boost::swap(n1,n2);
}

void main()
{
	Test1();
}
