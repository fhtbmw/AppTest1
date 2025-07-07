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

//ͬ����ʱ��
void TestAsio()
{
	io_service m_ios;	//asio�������Ҫ��io_service����
	deadline_timer dt(m_ios,boost::posix_time::seconds(2));		//�ȴ�2�����
	cout<<dt.expires_at()<<endl;	//��ӡ��������ʱ��

	dt.wait();		//����wait����ͬ���ȴ�
	cout<<"asio" << endl;
}

void CallFunc(const boost::system::error_code&)
{
	cout<<"callfunc" << endl;
}

//�첽��ʱ��
void TestAsio2()
{
	io_service m_ios;
	deadline_timer dt(m_ios,boost::posix_time::seconds(2));
	dt.async_wait(CallFunc);		//�첽�ȴ�

	cout<<",," << endl;
	m_ios.run();		//�첽IO��Ҫ�ĺ������ȴ����ؽ��
}

//void main()
//{
//	TestAsio2();
//}
