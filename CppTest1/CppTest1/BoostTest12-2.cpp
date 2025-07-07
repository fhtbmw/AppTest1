#ifdef _MSC_VER
#define _WIN32_WINNT 0X0501
#endif
#define BOST_REGEX_NO_LIB
#define BOOST_DATE_TIME_SOURCE
#define BOOST_SYSTEM_NO_LIB

#include "pch.h"
#include<iostream>
#include<system_error>
#include<boost/asio.hpp>
#include<boost/date_time/posix_time/posix_time.hpp>

using namespace std;
using namespace boost::asio;

//同步定时器
void TestAsio()
{
	io_service m_ios;	//asio程序必须要有io_service对象
	deadline_timer dt(m_ios,boost::posix_time::seconds(2));		//等待2秒结束
	cout<<dt.expires_at()<<endl;	//打印格林威治时间

	dt.wait();		//调用wait进行同步等待
	cout<<"asio" << endl;
}

void CallFunc(const boost::system::error_code&)
{
	cout<<"callfunc" << endl;
}

//异步定时器
void TestAsio2()
{
	io_service m_ios;
	deadline_timer dt(m_ios,boost::posix_time::seconds(2));
	dt.async_wait(CallFunc);		//异步等待

	cout<<",," << endl;
	m_ios.run();		//异步IO必要的函数，等待返回结果
}

//void main()
//{
//	TestAsio2();
//}
