#include <thread>
#include <mutex>
#include <functional>
#include <iostream>
#include <future>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uv.h>
#include <assert.h>
#include "pch.h"

#define _TESTTHREAD 1
#define _TESTLIBUV 2
#define _TESTLIBUV_PIPE 3

#define TEST _TESTLIBUV_PIPE

using namespace std;

#if TEST == _TESTTHREAD

void ThreadFuncFuture(std::future<int>& f)
{
	int _data;
	_data = f.get();

	cout << "get value " << _data << endl;
}

void ThreadFuncPromise(std::promise<int>& p)
{
	int _data = 0;
	p.set_value(_data);
}

void testFuturePromise()
{
	std::future<int> fu1;
	std::promise<int> pro1;

	fu1 = pro1.get_future();

	std::thread t1(std::bind(ThreadFuncFuture, std::ref(fu1)));
	std::thread t2(std::bind(ThreadFuncPromise, std::ref(pro1)));

	if (t1.joinable() && t2.joinable())
	{
		t1.join();
		t2.join();
	}
}

void testThreadPool()
{


}

void main()
{
	testFuturePromise();

}

#elif TEST == _TESTLIBUV

#define NUM 
int a = 0;	\
SOCKET s; \



void testlibuv()
{
	uv_loop_t* loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	uv_loop_init(loop);		//初始化

	printf("Now qutting \n");		
	uv_run(loop, UV_RUN_DEFAULT);	//函数按照的指定的

	uv_loop_close(loop);	//释放所有有关资源
	free(loop);	
}

int64_t numbers = 3;

void wait_for_a_while(uv_idle_t* handle)
{
	numbers++;
	if (numbers >= 10e6)
	{
		uv_idle_stop(handle);
	}

}

void testlibuv2()
{
	uv_idle_t idff;
	
	uv_idle_init(uv_default_loop(), &idff);
	uv_idle_start(&idff, wait_for_a_while);

	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_loop_close(uv_default_loop());
	return ;
}

uv_loop_t* _loop;

void alloc_buffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void on_read()
{
	do 
	{
		cout << "on_read \n";

	} while (0);

}

#elif TEST == _TESTLIBUV_PIPE

uv_loop_t* uvl;
uv_pipe_t uvp;

typedef struct
{
	uv_write_t req;
	uv_buf_t buf;
}write_req_t;


void conn_func(uv_connect_t* req, int status)
{
	printf("conn func \n");
}

void alloc_func(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void write_func(uv_write_t* req, int status)
{
	printf("write func \n");
}

void read_func(uv_stream_t* stream,
	ssize_t nread,
	const uv_buf_t* buf)
{
	write_req_t* writebuf = (write_req_t*)malloc(sizeof(write_req_t));
	assert(writebuf != nullptr);
	writebuf->buf = uv_buf_init(buf->base, nread);
	printf("read func: %s \n", writebuf->buf.base);
	uv_write((uv_write_t*)writebuf, (uv_stream_t*)&uvp, &writebuf->buf, 1, write_func);
}

void test_pipe_client()
{
	uv_connect_t uvc;


	const char* ccPipe = "\\\\.\\pipe\\MyPipe";
	const char* data;
	data = "Dsadasdasd";
	char recvdata[128];
	memset(recvdata, NULL, 128);
	recvdata[127] = '\0';
	DWORD dwWrittensize = 0, dwReadSize = 0;

	uvl = uv_default_loop();
	uv_pipe_init(uvl, &uvp, 1);

	//uv_pipe_connect(&uvc, &uvp, ccPipe, conn_func);
	uv_pipe_connect(&uvc, &uvp, ccPipe, NULL);
	if (!WriteFile(uvc.u.connect.pipeHandle, data, strlen(data), &dwWrittensize, NULL))
	{
		cerr << "client writefile failed error code : " << GetLastError() << endl;
		return;
	}
	if (!ReadFile(uvc.u.connect.pipeHandle, recvdata, 128, &dwReadSize, NULL))
	{
		cerr << "client readfile failed error code : " << GetLastError() << endl;
		return;
	}

	cout << recvdata << endl;
}


#define DEFINETEST(param1, param2, param3) \
do									\
{									\
	switch (param2-> param3-> type)			\
	{									\
	case T :							\
										\
	default:							\
		break;							\
	}									\
											\
								\
} while (0);							\


struct SS
{
	int age;
	char name;
};

void test()
{
	/*SS s1;
	SS* ps = CONTAINING_RECORD(&s1, SS, age);*/

	/*BYTE cbnum = 231;
	BYTE cbnum2 = 0xe7;*/

	vector<BYTE> vtn1;
	vtn1.resize(12);
	cout << sizeof(vtn1) << " " << vtn1.size();
}

class TC1
{
public:
	vector<BYTE> vt1;
};

void test2()
{
	TC1 tc1;
	tc1.vt1.resize(2);
	auto p = tc1.vt1.data();	//返回vector首地址

	vector<int> vt2(5, 1);
	vector<BYTE> vt3(5, 1);


	typedef struct haw_header_s {
		uint32_t magic;
		uint16_t version;
		uint16_t length;
		uint64_t name;
	}haw_header_t;

	cout << sizeof(haw_header_s);
}

void main()
{
	//test_pipe_client();
	test2();
}


#endif