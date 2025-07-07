#include"BoostTest6.h"


/*

boost����ļ�ϵͳ����ƽ̨��ʹ�ü���

*/

DWORD FileSysTest(LPVOID pragma) {
	path p("D:/t.t.txt");
	HFILE HF;
	if (::exists(p)) {
		cout<< "exists" << endl;
	}
	else {
		::create_directory(p);
		HF=::OpenFile("D:/t.t.txt",nullptr,0);
		::_lclose(HF);
	}
	return 0;
}

DWORD FileSysTest2() {
	path p="<><><";
	
	return 0;
}

class f {
public:
	void show_f();
	virtual void func() {
		return ;
	}
 };

class s :public f{
public:
	void func() {
		cout << "s";
	}
};

/*

boost ���߳�

*/

void TimerClassTest() {
	const posix_time::seconds time (2);
	posix_time::ptime cur_time;
	cur_time=get_system_time();
	this_thread::sleep(time);
	cur_time = get_system_time();
	cout << "2 seconds";
}

template<typename T>
class tt :noncopyable {
public:
	/*tt(const tt& in) {
		this->n = in.n;
	}*/
	tt(T x=T ()):n(x) {
		
	}
	T operator ++() {
		mutex::scoped_lock lock(mu);
		return ++n;
	}
	operator T(){
		return n;
	}

private:
	mutex mu;
	T n;
};

void MutexTest() {
	typedef tt<int> _int;
	_int tt1;
	cout << ++tt1;
}

mutex  g_c;

void ThreadInterrupt(const string&s) {
	try{
		for (int i = 0; i < 5; i++) {
			this_thread::sleep(posix_time::seconds(1));
			mutex::scoped_lock lock(g_c);
			cout<<" " << endl;
		}
	}
	catch (thread_interrupted&) {
		cout << "thread interrupted" << endl;
	}
}

void ThreadTest() {
	thread t(ThreadInterrupt,"a");
	this_thread::sleep(posix_time::seconds(2));
	t.interrupt();
	t.join();
}



/*
uuid��,����uuid��ȫ��Ψһ��ʶ����;
*/

//void UUID_Test() {
//	boost::uuids::uuid u;
//	assert(uuid::static_size()==16);
//	assert(u.size()==16);
//
//}

/*

boost assert 

*/

// BOOST_ENABLE_ASSERT_HANDLER������ʧ�ܺ����assertioj_failed��������ʽ�������

namespace boost {
	void assertion_failed(char const* expr,char const * function,char const* file,long line) {
		/*boost::format fmt("%s \n  %s\n %s \n %ld \n\n");
		fmt% expr% function% file% line;
		cout << fmt;*/
		string s;
		s += "Unknown Error";
		throw std::invalid_argument(s);
	}
}

int func(int a) {
	//BOOST_ASSERT(a > 0 && "a>0");
	BOOST_STATIC_ASSERT(sizeof(a)>3);	//�����ڶ���
	assert(a > 0);
	return 1 / a;
}

void TestAssert() {
	func(1);
}

/*

test��

*/
//
//#define BOOST_TEST_MAN
//#include<boost/test/included/unit_test.hpp>
//
////minimal test ����С����Ŀ ��Ԫ����
//
//#include<boost/test/minimal.hpp>

//int test_main(int argc,char* argv[]) {
//	format fmt("%d-%d");
//	BOOST_CHECK(fmt.size()!=0);
//	fmt % 12 % 34;
//	BOOST_REQUIRE(fmt.str()=="12-34");
//	BOOST_ERROR("��ʾ");
//	fmt.clear();
//	fmt % 12;
//	try {
//		cout << fmt;
//	}
//	catch (...) {
//		BOOST_FAIL("ERROR ");
//	}
//	return 0;
//}
















