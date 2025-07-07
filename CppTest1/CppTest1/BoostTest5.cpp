#include<windows.h>
#include<vector>
#include<iostream>
#include <boost/typeof/typeof.hpp>
#include<boost/container/detail/singleton.hpp>
#include<boost/serialization/singleton.hpp>
#include<boost/logic/tribool.hpp>
#include<boost/logic/tribool_io.hpp>

#ifdef SingleObj
using namespace boost::container::dtl;
using namespace boost::serialization;
#endif //SingleObj

using namespace std;
using namespace boost;

/*

单例模式类 
使用 singleton_object模板类创建

*/
  
#ifdef SingleObj
class Test {
public:
	Test(int a,char c):x(a),y(c) {
		cout<<"Create Obj" << endl;
	}
	Test() {
		cout<<"Crete Obj" << endl;
	}
	~Test() {
		cout << "Destroy Obj";
	}

	void Work() {
		HANDLE hile = CreateFile(
			L"D:\\Doctest.txt",
			GENERIC_WRITE|GENERIC_READ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
		);
		if (hile==INVALID_HANDLE_VALUE) {
			cerr << "create file failed "<<GetLastError() << endl;
			MessageBox(NULL,L"create file failed",L"Error",MB_ICONERROR);
			return;
		}
		const char *s = "Wawawa";
		DWORD written;
		WriteFile(hile,s,strlen(s),&written,NULL );
		
		SetFilePointer(hile,0,NULL,FILE_BEGIN);

		char buf[256];
		DWORD read;
		ReadFile(hile,buf,sizeof(buf),&read,NULL);
		cout<<buf<<endl;
		CloseHandle(hile);
	}

	int x;
	char y;
};

void SingleTonTest() {
	Test& t1 = singleton_default<Test>::instance();
	t1.Work();
}

/*

通过继承seilization 下的singletion类

*/
class Test2 :public singleton<Test2> {
	
};
#endif	//SingleObj

/*
	tribool类，三种状态:是、否、不确定
*/
void TriboolTest() {
	boost::tribool tb(1);
	tribool tb2(indeterminate);
	tribool tb3=(tb2||true);
	cout << tb3;
}

/*

	tokenizer库，处理字符串
	
*/

#include<boost/tokenizer.hpp>
using namespace boost;

void TestToken() {
	string _s{ "like app" };
	tokenizer<>t(_s);
	
	for (BOOST_AUTO(it, _s.begin());it!=_s.end();it++) {
		cout<<"{" <<* it<<"}";
	}
}















