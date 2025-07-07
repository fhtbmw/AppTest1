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

����ģʽ�� 
ʹ�� singleton_objectģ���ഴ��

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

ͨ���̳�seilization �µ�singletion��

*/
class Test2 :public singleton<Test2> {
	
};
#endif	//SingleObj

/*
	tribool�࣬����״̬:�ǡ��񡢲�ȷ��
*/
void TriboolTest() {
	boost::tribool tb(1);
	tribool tb2(indeterminate);
	tribool tb3=(tb2||true);
	cout << tb3;
}

/*

	tokenizer�⣬�����ַ���
	
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















