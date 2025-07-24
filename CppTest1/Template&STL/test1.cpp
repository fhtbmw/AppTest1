#include <vector>
#include <queue>
#include <list>
#include <algorithm>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <memory>
#include <functional>
#include <assert.h>
#include <numeric>

#define TEST_CONTAINER

using namespace std;


#ifdef TEST_RED_BLACK_TREE

//Red Black Tree

enum Color {Red, Black};

template<typename T>
struct Node
{
	T data;
	Color color;
	Node* left;
	Node* right;

};


template <typename T, size_t N>
class Array
{
public:

	T* begin() { return _array; }

	T* end() { return _array + N; }

	T* begin() const { return _array; }

	T* end() const { return _array + N; }

	T& operator[](size_t n) { return _array[n]; }

	const T& operator[](size_t n)const { return _array[n]; }

	size_t size() const { return N; }

	T _array[N];
};


template<typename T, size_t N>
bool operator == (const Array<T, N>a, const Array<T, N>b)
{
	for (size_t i =0; i< N; i++)
	{
		if (a[i] == b[i])
		{
			continue;
		}
		else
		{
			return false;
		}

	}

	return true;
}

template<typename T, size_t N>
bool operator < (const Array<T, N>a, const Array<T, N>b)
{
	for (size_t i = 0; i < N; i++)
	{
		if (a[i] < b[i])
		{
			return true;
		}
		else if(a[i] > b[i])
		{
			return false;
		}

	}

}

void test1()
{
	Array<int, 9> arr1{1};
	Array<int, 9> arr2{1};

	if (arr1 == arr2)
	{
		cout << "Equality \n";
	}
}

#endif

#ifdef TEST_SEQUENCE

#include <algorithm>  // std::copy, std::copy_backward, std::distance
#include <stdexcept>  // std::out_of_range
#include <iterator>   // std::iterator_traits

template<class T>
class _sequence
{
public:
	// 迭代器类
	class _Iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		_Iterator(T* ptr) : _ptr(ptr) {}

		// 解引用操作符
		reference operator*() const { return *_ptr; }

		// 前置递增
		_Iterator& operator++() { ++_ptr; return *this; }

		// 后置递增
		_Iterator operator++(int) { _Iterator tmp = *this; ++_ptr; return tmp; }

		// 不等比较
		bool operator!=(const _Iterator& other) const { return _ptr != other._ptr; }

		// 随机访问（用于计算距离）
		difference_type operator-(const _Iterator& other) const { return _ptr - other._ptr; }

		// 指针算术
		_Iterator operator+(difference_type n) const { return _Iterator(_ptr + n); }
		_Iterator& operator+=(difference_type n) { _ptr += n; return *this; }
		_Iterator operator-(difference_type n) const { return _Iterator(_ptr - n); }
		_Iterator& operator-=(difference_type n) { _ptr -= n; return *this; }

	public:
		T* _ptr;
	};

	// 构造函数
	_sequence() : data(nullptr), size(0), capacity(0) {}
	~_sequence() { delete[] data; }

	// 插入单个元素
	void insert(_Iterator it, const T& t)
	{
		insert(it, 1, t);
	}

	// 插入 n 个相同元素
	void insert(_Iterator it, size_t n, const T& t)
	{
		if (it._ptr < data || it._ptr > data + size)
		{
			throw std::out_of_range("Iterator out of range");
		}

		size_t pos = it._ptr - data;

		if (size + n > capacity)
		{
			size_t new_capacity = (capacity == 0) ? n : std::max(capacity * 2, capacity + n);
			T* new_data = new T[new_capacity];

			std::copy(data, data + pos, new_data);
			std::fill(new_data + pos, new_data + pos + n, t);
			std::copy(data + pos, data + size, new_data + pos + n);

			delete[] data;
			data = new_data;
			capacity = new_capacity;
		}
		else
		{
			std::copy_backward(data + pos, data + size, data + size + n);
			std::fill(data + pos, data + pos + n, t);
		}

		size += n;
	}

	//插入同类型容器中元素
	void insert(_Iterator it, _Iterator fir, _Iterator sec)
	{
		if (it._ptr < data || it._ptr > data + size)
		{
			throw std::out_of_range("Iterator out of range");
		}

		size_t pos = it._ptr - data;	//插入位置距容器起始位置的距离

		size_t num = sec - fir;

		if (num + size > capacity)		//需扩容
		{
			size_t new_capacity = (capacity == 0) ? num : std::max(2 * capacity, capacity + num);
			
			T* new_data = new T[new_capacity];

			std::copy(data, data + pos, new_data);

			/*for (size_t i = 0; i < n; i++)
			{

			}*/

		}

	}

	// 插入迭代器范围 [first, last)
	template <typename InputIterator>
	void insert(_Iterator it, _Iterator first, _Iterator last)
	{
		if (it._ptr < data || it._ptr > data + size)
		{
			throw std::out_of_range("Iterator out of range");
		}

		size_t pos = it._ptr - data;

		// 计算要插入的元素数量
		size_t n = std::distance(first, last);

		if (size + n > capacity)
		{
			// 计算新的容量（通常翻倍）
			size_t new_capacity = (capacity == 0) ? n : std::max(capacity * 2, capacity + n);
			T* new_data = new T[new_capacity];

			// 复制插入位置之前的元素
			std::copy(data, data + pos, new_data);

			// 插入 [first, last) 的元素
			for (size_t i = 0; i < n; ++i)
			{
				new_data[pos + i] = *first;
				++first;
			}

			// 复制插入位置之后的元素
			std::copy(data + pos, data + size, new_data + pos + n);

			delete[] data;
			data = new_data;
			capacity = new_capacity;
		}
		else
		{
			// 移动现有元素，为新元素腾出空间
			std::copy_backward(data + pos, data + size, data + size + n);

			// 插入 [first, last) 的元素
			size_t i = 0;
			for (; first != last; ++first, ++i)
			{
				data[pos + i] = *first;
			}

			size += n;
		}
	}

	// 迭代器相关方法
	_Iterator begin() { return _Iterator(data); }
	_Iterator end() { return _Iterator(data + size); }

private:
	T* data;
	size_t size;
	size_t capacity;
};


//template<typename T>
//class MyIterator
//{
//public:
//
//	using value_type = T;
//	using difference_type = std::ptrdiff_t;
//	using pointer = T*;
//	using reference = T&;
//
//	MyIterator(T* ptr) :_ptr(ptr) {}
//
//	T& operator* () const { return *_ptr; }
//
//	MyIterator& operator++ () { ++_ptr; return *this; }
//
//	bool operator!= (const MyIterator& other) const { return _ptr != other._ptr; }
//
//private:
//	T* _ptr;
//};


void test2()
{
	int ia[5] = { 2,3,2,3,4 };
	vector<int> vtN1;
	//std::copy(ia, ia + 5, vtN1.begin());	//copy复制元素到已有位置,vector空间未初始化,报错
	copy(ia, ia + 5, back_inserter(vtN1));	//
}

void test3()
{
	std::queue<char> q;
	std::list<char> l;
	std::vector<char> vt;

	l.insert(l.begin(), 2, 'c');
	vt.insert(vt.begin(), l.begin(), l.end());

}

#include <iostream>
#include <iterator>

template <typename T>
class MyIterator {
public:
	using value_type = T;
	using difference_type = std::ptrdiff_t;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::random_access_iterator_tag;

	MyIterator(T* ptr) : ptr_(ptr) {}

	T& operator*() const { return *ptr_; }
	MyIterator& operator++() { ++ptr_; return *this; }
	bool operator!=(const MyIterator& other) const { return ptr_ != other.ptr_; }

private:
	T* ptr_;
};

int m1() {
	int arr[] = { 1, 2, 3, 4, 5 };
	MyIterator<int> begin(arr);
	MyIterator<int> end(arr + 5);

	// 使用 std::iterator_traits 提取类型信息
	using Traits = std::iterator_traits<MyIterator<int>>;

	//using ValueType = typename Traits::value_type;
	//using DifferenceType = typename Traits::difference_type;

	//std::cout << "Value type: " << typeid(ValueType).name() << std::endl; // int
	//std::cout << "Difference type size: " << sizeof(DifferenceType) << " bytes" << std::endl; // 8 bytes

	return 0;
}

template<typename T>
void tt(T t)
{
	typename std::iterator_traits<T>::value_type vtp;
}

#include <iostream>
#include <iterator>
#include <vector>
#include <list>


template<class BidirIt>
void my_reverse(BidirIt first, BidirIt last)
{
	typename std::iterator_traits<BidirIt>::difference_type n = std::distance(first, last);
	--n;
	while (n > 0) {
		typename std::iterator_traits<BidirIt>::value_type tmp = *first;
		*first++ = *--last;
		*last = tmp;
		n -= 2;
	}
}

int m2()
{
	std::vector<int> v{ 1, 2, 3, 4, 5 };
	my_reverse(v.begin(), v.end());
	for (int n : v) {
		std::cout << n << ' ';
	}
	std::cout << '\n';

	std::list<int> l{ 1, 2, 3, 4, 5 };
	my_reverse(l.begin(), l.end());
	for (auto n : l) {
		std::cout << n << ' ';
	}
	std::cout << '\n';

	int a[] = { 1, 2, 3, 4, 5 };
	my_reverse(a, a + 5);
	for (int i = 0; i < 5; ++i) {
		std::cout << a[i] << ' ';
	}
	std::cout << '\n';

	//    std::istreambuf_iterator<char> i1(std::cin), i2;
	//    my_reverse(i1, i2); // compilation error
	return 0;
}

int m3()
{
	std::list<int> ll{2, 3, 5, 6, 67};
	list<int>::iterator it = std::find(ll.begin(), ll.end(), 5);
	my_reverse(it, ll.end());
	for (auto i : ll)
	{
		cout << i << endl;
	}

	pair<int*, ptrdiff_t> p = get_temporary_buffer<int>(100);	//返回值第一个个为指向分配的内存的指针，第二个为该缓冲区的大小
	int* buf = p.first;

	return_temporary_buffer(buf);

	return 0;
}

void m4()
{
	typedef std::pair<int, const char*> _pair;

	std::vector<_pair> vt1;
	vt1.push_back(_pair(1, "a"));
	vt1.push_back(_pair(2, "b"));



}

#endif

#ifdef TEST_BASICCOMPONENTS 
class Int 
{
public:
	Int(int x) :val(x)
	{
	
	}

	int get()
	{
		return val;
	}

private:
	int val;

};

void m5()
{
	Int in(23);
	Int* a = (Int*)malloc(10 * sizeof(Int));
	uninitialized_fill_n(a, 10, in);
	size_t ssize = 10*sizeof(Int);
	cout << ssize << endl;
}


void m6()
{
	std::pair<int*, std::ptrdiff_t> p = get_temporary_buffer<int>(2);
	uninitialized_fill_n(p.first, p.second, 2);

	/*if (p.first)
	{
		for (int i = 0; i < p.second; i++)
		{
			cout << p.first[i] << endl;
		}
		cout << p.second << endl;
	}*/

	//find_if(p.first, p.first + p.second, std::bind(less<int>(), 3));

	vector<int> vt1{2,3,4,3};
	
	find_if( vt1.begin(), vt1.end(), std::bind2nd(less<int>(), 3));

	auto pr = std::adjacent_find(vt1.begin(), vt1.end());
	
	if (pr != vt1.end())
	{
		cout << *pr << endl;
	}
	else
	{
		pr = std::adjacent_find(vt1.begin(), vt1.end(), greater<int>());
		cout << *pr << endl;
	}

	return_temporary_buffer(p.first);
}

template<class T>
class AA
{
public:

	AA(T t) :n(t) {}

	bool operator()(T a, T b)
	{
		return (a - b) % n == 0;
	}

private:
	T n;
};


void m7()
{
	vector<int> vt1{ 4,4,1,3,3,434, };
	vector<int> vt2{ 4,4,1,3,3,434, };
	int i1 = 1;

	auto p = std::find_first_of(vt1.begin(), vt1.end(),vt2.begin(), vt2.end());
	if (p != vt1.end())
	{
		cout << *p << endl;
	}
}

#endif


#ifdef TEST_BASICMUTATINGALGORITHMS

template<class Type>
void Print(Type& item)
{
	typename Type::iterator _iterator;

	for (_iterator = item.begin(); _iterator != item.end(); _iterator++)
	{
		cout << *_iterator << " ";
	}
	cout << "\n";
}

template<class T>
void Print2(T t)
{
	for (auto it = t.begin(); it != t.end(); it++)
	{
		cout << *it;
	}
	cout << "\n";
}

void TestCopy()
{
	std::vector<char> vtChar1{ 'd','l','l' };
	std::vector<char> vtChar2;
	vtChar2.resize(vtChar1.size());	//需分配足够空间才能接受copy的内容，或者使用back_inster()

	std::copy(vtChar1.begin(), vtChar1.end(), vtChar2.begin());
	std::vector<char>::iterator it;

	Print<std::vector<char>>(vtChar2);
	
	int anaylise;
	std::list<int> lin1;
	lin1.resize(5);
	Print2<std::list<int>>(lin1);

}

void test2()
{
	char a[] = "Dsd";
	char b[] = "Dsda";
	vector<char>vtA(a, a + strlen(a));
	vector<char>vtB(b, b + strlen(b));
	list<char> lA(vtA.size());
	
	copy(vtA.begin(), vtA.end(), lA.begin());

	assert(equal(vtA.begin(), vtA.end(), lA.begin()));

	copy(vtB.begin(), vtB.end(), ostream_iterator<char>(cout));
}

void test3()
{
	int ia[6] = {2,4,3,4,34,2};

	std::copy_backward(ia, ia+3, ia+6);
	
	std::copy(ia, ia+6, ostream_iterator<int>(cout, " "));
}

//TestADL：参数依赖查找，根据函数参数，扩大调用函数的查找范围


#include <iostream>  // for std::cout

namespace _std
{
	/// 模拟 std 中的模板函数
	template<typename Tp>
	void fun(const Tp&) {
		std::cout << "Call fun() in _std.\n";
	}
}

/// 一个特殊的类型
struct myType { };

/// 对 myType 进行特化
void fun(const myType&) {
	std::cout << "Call fun() specially for myType.\n";
}

template<typename T>
struct test {

	T x;
	/// 不使用 using 技法
	void fun1() {
		std::cout << "fun1, without 'using':\t";
		_std::fun(x);
	}

	/// 使用 using 技法
	void fun2() {
		std::cout << "fun2, with 'using':\t";
		using _std::fun;
		fun(x);
	}
};

int test4()
{
	test<int> t_int;       /// 用于对照
	test<myType> t_myType;

	std::cout << "t_int:\n";
	t_int.fun1();
	t_int.fun2();

	std::cout << "\nt_myType:\n";
	t_myType.fun1();
	t_myType.fun2();

	return 0;
}

void test5()
{
	vector<int> vt1{ 2,2,3,3 ,1};
	vector<int> vt2{ 4,5,32,4 };
	vt2.resize(vt1.size() + vt2.size());

	swap_ranges(vt1.begin(), vt1.end(), std::find(vt2.begin(), vt2.end(), 32));

	std::copy(vt2.begin(), vt2.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

	const int N = 10;
	vector<int> vt3(N);
	std::fill(vt3.begin(), vt3.end(), 23);
	std::fill_n(back_inserter(vt3), 5, 0);	//在vt3末尾填充数据	

	int A[N] = { 3,4,4,3,323,1 };

	std::transform(vt3.begin(), vt3.end(), A,
		ostream_iterator<int>(cout, " "), plus<int>());	//First1 Fitst2 进行Func运算，再赋值给Dest

	cout << endl;

	std::generate(vt3.begin(), vt3.end(), rand);	//将输入范围内元素用rand()替换

	std::generate_n(back_inserter(vt3), 10, rand);

	std::copy(vt3.begin(), vt3.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

	vector<int>::iterator it = std::remove(vt1.begin(), vt1.end(), 4);	//remove

	std::copy(vt1.begin(), vt1.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

	vt1.erase(std::remove(vt1.begin(), vt1.end(), 3), vt1.end());	//vector移除元素

	std::copy(vt1.begin(), vt1.end(), ostream_iterator<int>(cout, " "));

	cout << endl;
	 
	std::remove_copy(vt1.begin(), vt1.end(), ostream_iterator<int>(cout, " "), 0); //将容器内除了val之外的数据赋值给Dest
	 
	cout << endl;
}

void test5_1()
{
	std::vector<int> vtNum{ 2,3,4,32,4,324233,2 };

	std::sort(vtNum.begin(), vtNum.end());

	vtNum.erase(std::unique(vtNum.begin(), vtNum.end()), vtNum.end());	//unique移除相邻重复元素，返回新迭代器

	std::copy(vtNum.begin(), vtNum.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

	std::reverse(vtNum.begin(), vtNum.end());

	std::copy(vtNum.begin(), vtNum.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

	std::rotate(vtNum.begin(), vtNum.begin() + (vtNum.size() / 2), vtNum.end());

	std::copy(vtNum.begin(), vtNum.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

}


void test6()
{
	vector<int> vt1(2, 454);
	
	vector<int> vt2(3,0);
	
	vector<int>::iterator it = vt2.begin() + 2;

	std::copy_backward(vt1.begin(), vt1.end(), it);	//Dest iterator在Dest begin()后

	std::replace(vt1.begin(), vt1.end(), 454, 1);

	std::copy(vt1.begin(), vt1.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

	std::copy(vt2.begin(), vt2.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

	std::swap_ranges(vt1.begin(), vt1.end(), vt2.begin());
	//std::swap_ranges(vt2.begin(), vt2.end(), vt1.begin());	//First2 容器大小大于 First1

	std::copy(vt2.begin(), vt2.end(), ostream_iterator<int>(cout, " "));

	cout << endl;

}

void test6_1()
{
	vector<int> vt1{ 1,2,3,4,5 };
	vector<int> vt2{ 2,3,4,5,3 };

	auto start_iterator = vt1.begin();
	auto back_iter = std::back_inserter(vt2);

	cout << _ITERATOR_DEBUG_LEVEL << endl;
	cout << _Unwrappable_for_unverified_v<decltype(start_iterator)> << endl;
	cout << _Unwrappable_for_unverified_v<decltype(back_iter)> << endl;

}


#endif


#ifdef SORT_SEARCH


void test7()
{
	vector<string> vtV1{"Apple, Banana, Pear, Mango"};

	std::sort(vtV1.begin(), vtV1.end(), greater<string>());
	
	assert(is_sorted(vtV1.begin(), vtV1.end()));

	std::copy(vtV1.begin(), vtV1.end(), ostream_iterator<string>(cout, "\n"));

	std::stable_sort(vtV1.begin(),vtV1.end());
	
	std::stable_sort(vtV1.begin(),vtV1.end(), greater<string>());
}

class MyObj 
{
public:
	enum ep {standby, normal, high, urgant};

	MyObj(string id, ep p = urgant):nam(id), epi(p) {}

	ep get()const { return epi; }

	string name() const { return nam; }

private:

	string nam;
	ep epi;

};

 ostream& operator << (ostream& s,const MyObj& j)
{
	s << j.name() << j.get() << "\n";
	return s;
}

 bool operator < (const MyObj&o1, const MyObj& o2)
{
	return o1.get() < o2.get();
}

void test8()
{
	vector<MyObj> vtjobs{MyObj("dasd", MyObj::high), MyObj("dsadsd", MyObj::normal), MyObj("dwd", MyObj::standby)};

	stable_sort(vtjobs.begin(), vtjobs.end());

	copy(vtjobs.begin(), vtjobs.end(), ostream_iterator<MyObj>(cout, "\n"));

	vector<MyObj> vtjobs2{ MyObj("dasd", MyObj::high), MyObj("dsadsd", MyObj::normal), MyObj("dwd", MyObj::standby) };

	partial_sort(vtjobs2.begin(), vtjobs2.begin() + 2, vtjobs2.end());

	copy(vtjobs2.begin(), vtjobs2.end(), ostream_iterator<MyObj>(cout, "\n"));

	vector<int> vtNum{ 44,22,33,3,3,1231231223,22,4343,9,1,2,3,3,2,3 };

	partial_sort(vtNum.begin(), vtNum.begin() + 5, vtNum.end());	//begin ~ middle之间元素有序

	copy(vtNum.begin(), vtNum.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	vector<int>vtRecv(5);

	partial_sort_copy(vtNum.begin(), vtNum.begin() + 5, vtRecv.begin(), vtRecv.end());

	copy(vtRecv.begin(), vtRecv.end(), ostream_iterator<int>(cout, " "));
	cout << endl;

	nth_element(vtNum.begin(), vtNum.begin() + 5, vtNum.end());		//begin~middle 间元素(begin~middle间顺序无保证) < middle ~ last间元素

}

void test9()
{
	vector<int> vtTest{3,4,3,4,3,4,56,4};

	bool ret= binary_search(vtTest.begin(), vtTest.end(), 5);

	_STD copy(vtTest.begin(), vtTest.end(), ostream_iterator<int>(cout, " \n"));
	cout << endl;

	std::lower_bound(vtTest.begin(), vtTest.end(), 5);
	
	vector<int> vtSorted1{ 0,3,4,5,6, 2,9,8 };

	std::inplace_merge(vtSorted1.begin(), vtSorted1.begin() + 5, vtSorted1.end());	//区间begin~mid有序,输入的区间需有序

	_STD copy(vtSorted1.begin(), vtSorted1.end(), ostream_iterator<int>(cout, " \n"));
}

void test10()
{
	int iA[] = { 3,4,34,4,7 };

	const int N = sizeof(iA) / sizeof(int);

	make_heap(iA, iA + N);

	copy(iA, iA+N, ostream_iterator<int>(cout, " "));
	cout << endl;

	assert(is_heap(iA, iA + N));

	std::sort_heap(iA, iA + N);		//排序后不再保持堆结构
	copy(iA, iA + N, ostream_iterator<int>(cout, " "));
	cout << endl;


}

#endif

template<typename _IN>
void OutPut(_IN start, _IN end)
{
	std::copy(start, end, ostream_iterator<int>(cout, " "));
	cout << endl;
}

#ifdef TEST_ITERATOR

void test11()
{
	list<int> l1{1,3,5};

	insert_iterator<list<int>> li(l1, l1.end());

	*li++ = 7;
	*li++ = 9;

	/*std::copy(l1.begin(), l1.end(), ostream_iterator<int>(cout, " "));
	cout << endl;*/

	OutPut<list<int>::iterator>(l1.begin(), l1.end());

	reverse_copy(l1.begin(), l1.end(), ostream_iterator<int>(cout, " "));

}

void test12()
{
	const int N = 22;
	int ia[N] = { 3,12,3,23,12,23,12,2 };

	list<int>l1{ ia, ia + 6 };

	std::copy(reverse_iterator<list<int>::iterator>(l1.end()),
			reverse_iterator<list<int>::iterator>(l1.begin()),	//倒序复制到输出迭代器，first last 颠倒
			ostream_iterator<int>(cout, " "));
	cout << endl;

	auto pos = std::find(reverse_iterator<list<int>::iterator>(l1.end()),
						reverse_iterator <list<int>::iterator>(l1.begin()),
						12).base();		//base: 前移一个位置

	cout << *pos << endl;
	
	list<int>::reverse_iterator::value_type;

}

void test13()
{
	vector<int> vtNum = { 2,3,4,3 };
	vector<int> vtNum2 = { -2,-3,-4,-3 };
	vector<int> vtNum3 = { -2,-3,-4,-3 };
	

	int* a = (int*)malloc(vtNum.size() * sizeof(int));

	transform(vtNum.begin(), vtNum.end(),
		raw_storage_iterator<int*, int>(a),
		negate<int>());

	transform(vtNum.begin(), vtNum.end(),
		vtNum2.begin(), vtNum3.begin(),
		plus<int>());

	partial_sum(vtNum2.begin(), vtNum2.end(), 
		vtNum3.begin(), multiplies<int>());

	transform(vtNum3.begin(), vtNum3.end(),
		vtNum3.begin(), bind2nd(modulus<int>(), 2));	//modulus 将区间内元素替换成取模后的余数

	partition(vtNum.begin(), vtNum.end(), bind2nd(equal_to<int>(), 0));		//partition将满足func的元素排在不满足元素之前
	
	//partition(vtNum.begin(), vtNum.end(), bind2nd(equal<int>(), ));


}

void test14()
{
	vector<int>vtnum1{ 0,1,2,3,4,0,0,0 };

	vtnum1.erase(std::remove(vtnum1.begin(), vtnum1.begin()+5, 0), vtnum1.end());

	std::copy(vtnum1.begin(), vtnum1.end(), ostream_iterator<int>(cout, ""));
	std::cout << endl;

	transform(vtnum1.begin(), vtnum1.end(), vtnum1.begin(), negate<int>());

	auto pos = partition(vtnum1.begin(), vtnum1.end(),
		[](int n)
		{
			return n < 0;
		});

	vtnum1.erase(pos, vtnum1.end());
	
	list<uint8_t> l1{0,0,0x03,0x02,0x03,0,0,0xac};
	list<int> l2{0,0,3,2,3,0,0,9};

	list<uint8_t>::iterator it = std::find_if(l1.begin(), l1.end(), bind2nd( not_equal_to<uint8_t>(),0));

	std::copy(it, l1.end(), ostream_iterator<int>(cout, ""));
	std::cout << endl;

	std::sort(vtnum1.begin(), vtnum1.end(), less<int>());
	std::copy(vtnum1.begin(), vtnum1.end(), ostream_iterator<int>(cout, " "));

}
	
class Base
{
public:
	virtual void print() = 0;
};

class C1 : public Base
{
public:
	
	virtual void print()
	{
		cout << "C1 \n";
	}

};

class C2 : public Base 
{
public:

	virtual void print()
	{
		cout << "C2 \n";
	}

};

void test15()
{
	hash<const char*>h1;
	cout << " f -> " << h1("f") << endl;
	cout << " h -> " << h1("h") << endl;

	vector<Base*>vt1{new C2};
	vt1.push_back(new C1);

	for_each(vt1.begin(), vt1.end(), mem_fun(&Base::print));
}

#endif

#ifdef TEST_CONTAINER


void test16()
{
	vector<int> vtnum1{ 1,2,3,5,0,-2,4 };
	vector<int>::iterator it = std::find(vtnum1.begin(), vtnum1.end(), -2);
	
	it = vtnum1.begin();

	cout << vtnum1.data() << endl;

	vtnum1.push_back(3);
	vtnum1.push_back(6);
	vtnum1.push_back(33);

	cout << vtnum1.data() << endl;  // 添加新元素，重新分配地址给容器，之前的迭代器失效
	
	typedef typename vector<int>::value_type ty;

	cout << typeid(ty).name() << endl;

	cout << "front " << vtnum1.front() << " back " << vtnum1.back();

	list<int> l1{ -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13, -14, -15, -16, -17, -18, -19, -20 };


	vtnum1.assign(l1.begin(), l1.end());
	cout << "front " << vtnum1.front() << " back " << vtnum1.back();

}


#endif // TEST_CONTAINER

void main()
{
	//tt();
	//test5_1();
	//test15();
	test16();
}


	