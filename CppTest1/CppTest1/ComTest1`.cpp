#include<iostream>
#include <objbase.h>
using namespace std;

//class IA {
//public:
//	virtual void Fa1() = 0;
//	virtual void Fa2() = 0;
//};
//
//class IB {
//public:
//	virtual void Fb1() = 0;
//	virtual void Fb2s() = 0;
//};
//
//class C :public IA,public IB {
//	
//	virtual void Fa1() 
//	{
//		cout << "Fa1" << endl;
//	}
//
//	virtual void Fa2()
//	{
//		cout<<"Fa2" << endl;
//	}
//
//	virtual void Fb1()
//	{
//		cout << "Fb1" << endl;
//	}
//
//	virtual void Fb2()
//	{
//		cout << "Fb2" << endl;
//	}
//
//};

void Print(const char *msg)
{
	cout << msg<<endl;
}

interface Ix
{
	virtual void _stdcall Fx1()=0;
};

interface IY
{
	virtual void _stdcall Fy1()=0;
};

class ca :public Ix, public IY 
{
	virtual void _stdcall Fx1()
	{
		cout<<"Fx1" << endl;;
	}

	virtual void _stdcall Fy1()
	{
		cout << "Fy1" << endl;;
	}
	
};

class MyComObj :public IUnknown {
public:
	MyComObj():refCount(1)
	{
	}

	//实现IUnknown三种基础方法
	HRESULT QueryInterface(REFIID riid,void **ppvObject ) override
	{
		if (riid==IID_IUnknown)
		{
			*ppvObject = static_cast<IUnknown*>(this);
			AddRef();
			return S_OK;
		}
		*ppvObject = NULL;


		return E_NOINTERFACE;
	}

private:
	long refCount;

};


class T1
{
public:
	T1():m(0)
	{
	
	}
private:
	int m;
};

//
//int main()
//{
//	ca* ca1 = new ca();
//
//	Ix* ix1 = ca1;
//	IY* iy1 = ca1;
//
//	Print("Client:Use Interface IX");
//	ix1 = ca1;
//	ix1->Fx1();
//
//	Print("Client:Use Interface Iy");
//	iy1 = ca1;
//	iy1->Fy1();
//	delete(ca1);
//	return 0;
//}
//





