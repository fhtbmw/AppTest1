#include <iostream>
#include <memory.h>
#include <assert.h>
#include <windows.h>

#define TEST_SMART_POINTER

using namespace std;

#ifdef TEST_SMART_POINTER

void test1()
{
	std::shared_ptr<int> sp1(new int(1));

	std::shared_ptr<int> sp2;
	sp2 = std::make_shared<int>(2);

	std::shared_ptr<int> sp3(sp2);
	cout << sp2.use_count() << endl;

	sp3.reset();

	cout << sp2.use_count() << endl; 

}

class TestSharedPtr : public std::enable_shared_from_this<TestSharedPtr>
{
public:

	void func()
	{
		std::shared_ptr<TestSharedPtr> sp = shared_from_this();
	}

};

wchar_t g_wcstr[50];

class TestSharedPtr2 : public std::enable_shared_from_this<TestSharedPtr2>
{
public:

	TestSharedPtr2()
	{
		swprintf(g_wcstr, 50, L"TestSharedptr2 Construct \n");
		OutputDebugStringW(g_wcstr);
	}

	~TestSharedPtr2()
	{
		swprintf(g_wcstr, 50, L"TestSharedptr2 Destruct \n");
		OutputDebugStringW(g_wcstr);
	}

	void func()
	{
		m_sptr = shared_from_this();
	}

private:
	int								m_iparam;
	std::shared_ptr<TestSharedPtr2>	m_sptr;

};

void test2()
{

#define T2

#ifdef T1

	std::shared_ptr<TestSharedPtr> sp1 = std::make_shared<TestSharedPtr>();
	sp1->func();

	wchar_t wc[50];

	swprintf(wc, 50, L"use_count: %d \n", sp1.use_count());

	OutputDebugStringW(wc);
#endif

#ifdef T2

	std::shared_ptr<TestSharedPtr2> sp1(new TestSharedPtr2());
	sp1->func();

#endif

}

#endif

void main()
{
	test2();
}
