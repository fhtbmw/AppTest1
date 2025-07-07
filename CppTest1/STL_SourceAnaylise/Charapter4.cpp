#include <iostream>
#include <list>
#include <algorithm>

#define TEST_LIST

using namespace std;

#ifdef TEST_LIST

template<typename T>
void _print(list<T>& lin)
{
	typename list<T>::iterator _iterator;
	for (_iterator = lin.begin(); _iterator != lin.end(); _iterator++)
	{
		cout << *_iterator << endl;
	}
	cout << endl;
}

void testlist()
{
	list<int> llist1;
	llist1.push_back(0);
	llist1.push_back(1);
	llist1.push_back(2);
	llist1.push_back(3);
	llist1.push_back(4);

	list<int>::iterator lit;

	_print<int>(llist1);

	lit = std::find(llist1.begin(), llist1.end(), 4);
	if (lit != llist1.end())
	{
		if (llist1.erase(lit) != llist1.end())
		{
			cout << *(llist1.erase(lit));	//返回删除元素下一个元素的迭代器
		}
	}
	cout << endl;

	list<int> llist2;
	llist2.resize(llist1.size() + 1);

	std::copy(llist1.begin(), llist1.end(), llist2.begin());
	llist2.insert(std::find(llist2.begin(), llist2.end(), 3), 255);		//在指定元素前一位插入
	llist2.insert(llist2.end()--, 255);

	_print<int>(llist1);
	_print<int>(llist2);

	llist1.clear();

	_print<int>(llist1);

}

std::ostream& operator<<(std::ostream& ostr, const std::list<int>& list)
{
	for (auto& i : list)
		ostr << ' ' << i;

	return ostr;
}

void testlist2()
{
	std::list<int> list1{ 1, 2, 3, 4, 5 };
	std::list<int> list2{ 10, 20, 30, 40, 50 };

	auto it = list1.begin();
	std::advance(it, 2);

	list1.splice(it, list2);

	std::cout << "list1:" << list1 << '\n';
	std::cout << "list2:" << list2 << '\n';

	list2.splice(list2.begin(), list1, it, list1.end());

	std::cout << "list1:" << list1 << '\n';
	std::cout << "list2:" << list2 << '\n';
}
	
#endif

void main()
{
	testlist2();
}







