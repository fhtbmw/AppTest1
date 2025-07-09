#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <uv.h>
#include <iostream>
#include <vector>
#include <windows.h>
#include <stdint.h>

#define TEST_SIGNAL

using namespace std;

#ifdef TEST_FILE

void on_read(uv_fs_t* req);

uv_fs_t open_req;
uv_fs_t read_req;
uv_fs_t write_req;

static char buffer[1024];

static uv_buf_t iov;

void on_write(uv_fs_t* req) {
	if (req->result < 0) {
		fprintf(stderr, "Write error: %s\n", uv_strerror((int)req->result));
	}
	else {
		uv_fs_read(uv_default_loop(), &read_req, open_req.result, &iov, 1, -1, on_read);
	}
}

void on_read(uv_fs_t* req) {
	if (req->result < 0) {
		fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
	}
	else if (req->result == 0) {
		uv_fs_t close_req;
		// synchronous
		uv_fs_close(uv_default_loop(), &close_req, open_req.result, NULL);
	}
	else if (req->result > 0) {
		iov.len = req->result;
		uv_fs_write(uv_default_loop(), &write_req, 1, &iov, 1, -1, on_write);
	}
}

void on_open(uv_fs_t* req) {
	// The request passed to the callback is the same as the one the call setup
	// function was passed.
	assert(req == &open_req);
	if (req->result >= 0) {
		iov = uv_buf_init(buffer, sizeof(buffer));
		uv_fs_read(uv_default_loop(), &read_req, req->result,
			&iov, 1, -1, on_read);
	}
	else {
		fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
	}
}

void on_open2(uv_fs_t* req)
{
	if (req->result > 0)
	{
		cout << " open file success \n";
	}
	else 
	{
		cout << " open file failed: " << uv_strerror((int)req->result);
	}
}

void test()
{
	const char* path = "./t.txt";
	uv_fs_open(uv_default_loop(), &open_req, path, O_RDONLY, 0, on_open);
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_fs_req_cleanup(&open_req);
	uv_fs_req_cleanup(&read_req);
	uv_fs_req_cleanup(&write_req);
}


#endif

#ifdef TEST_SIGNAL

#include <iostream>
#include <uv.h>

using namespace std;

// 创建并初始化 uv_loop_t
uv_loop_t* createloop() {
	uv_loop_t* uvloop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	if (!uvloop) {
		cerr << "Failed to allocate uv_loop_t" << endl;
		return nullptr;
	}
	uv_loop_init(uvloop);
	return uvloop;
}

// 释放 uv_loop_t
void destroyloop(uv_loop_t* loop) {
	if (loop) {
		uv_loop_close(loop);
		free(loop);
	}
}

// 信号回调函数
void SignalFunc(uv_signal_t* handle, int signum) {
	cout << "Receive signal: " << signum << endl;
	uv_signal_stop(handle);  // 停止信号监听
}

// 线程函数 1
void ThreadFunc1(void* arg) {
	uv_loop_t* lp = createloop();
	if (!lp) return;

	uv_signal_t st1;
	uv_signal_init(lp, &st1);
	uv_signal_start(&st1, SignalFunc, SIGINT);  // 使用 SIGINT (Ctrl+C)

	cout << "Thread 1: Waiting for signals..." << endl;
	uv_run(lp, UV_RUN_DEFAULT);  // 运行事件循环

	destroyloop(lp);
}

// 线程函数 2
void ThreadFunc2(void* arg) {
	uv_loop_t* lp = createloop();
	if (!lp) return;

	uv_signal_t st1;
	uv_signal_init(lp, &st1);
	uv_signal_start(&st1, SignalFunc, SIGTERM);  // 使用 SIGTERM

	cout << "Thread 2: Waiting for signals..." << endl;
	uv_run(lp, UV_RUN_DEFAULT);  // 运行事件循环

	destroyloop(lp);
}

// 测试函数
void test1() {
	uv_thread_t t1, t2;

	// 创建两个线程，分别运行不同的事件循环
	uv_thread_create(&t1, ThreadFunc1, nullptr);
	uv_thread_create(&t2, ThreadFunc2, nullptr);

	// 等待线程结束
	uv_thread_join(&t1);
	uv_thread_join(&t2);
}

#endif

volatile int g_flag = 0;

void singfunc(int sig)
{
	g_flag = 1;
	return;
}

void testsignal()
{
	signal(SIGINT, singfunc);
	
	while (true)
	{

	}

	if (g_flag == 1)
	{
		cout << "Exit \n";
	}
}

void HexToAscll(std::vector<uint8_t>vin)
{
	/*std::string s1;

	for (auto n : vin)
	{
		if (n >= 32 && n < 127)
		{
			s1 += n;
		}
	}*/
	for (auto i : vin) {
		std::cout << "Type: " << typeid(i).name() << ", Value: " << static_cast<int>(i) << std::endl;
	}

	//constexpr uin

	vin.erase(std::remove_if(vin.begin(), vin.end(), [](uint8_t i)
		{
			return 0;
		}),
		vin.end());

	/*vin.erase(std::remove(vin.begin(), vin.end(), 0),
		vin.end());*/
}

void Func(IN const LPBYTE _lpcbData, IN const DWORD _dwDataSize)
{
	//BYTE* pdbDevInfo = new BYTE[_dwDataSize];
	std::vector<BYTE> vtData(_dwDataSize, 0);
	//pdbDevInfo = (BYTE*)_lpcbData;
	memcpy(vtData.data(), _lpcbData, _dwDataSize);
	
	//_RPTFW1(_CRT_WARN, L" \n DevInfo: %s", pdbDevInfo);


	/*BYTE* cbString = new BYTE[_dwDataSize];

	memcpy(cbString, _lpcbData, _dwDataSize);

	wchar_t wstrDebug[32] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)cbString, -1, wstrDebug, _countof(wstrDebug));
	_RPTFW1(_CRT_WARN, L" \n DevInfo: %s", wstrDebug);

	delete[] cbString;*/

	BYTE cbData[64] = {0};

	memcpy(cbData, _lpcbData, _dwDataSize);

	wchar_t wstrDebug[32] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)cbData, -1, wstrDebug, _countof(wstrDebug));
	_RPTFW1(_CRT_WARN, L" \n DevInfo: %s", wstrDebug);


}

void testTrans()
{
	std::vector<uint8_t> vtNum1{0x04, 0xc3, 0x01,0x4d, 0x2d, 0x56, 0x4d, 0x38, 0x30, 0x30, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	vtNum1.erase(std::remove_if(vtNum1.begin(), vtNum1.end(), [](uint8_t i)
		{
			return ((i == 0x00) || (i < 0x20) || (i >= 0x7f));
		}),
		vtNum1.end());
	Func(vtNum1.data(), vtNum1.size());
}

void main()
{
	/*std::vector<uint8_t> vtNum1{0x04, 0xc3, 0x01,0x4d, 0x2d, 0x56, 0x4d, 0x38, 0x30, 0x30, 0x4c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	vtNum1.erase(std::remove(vtNum1.begin(), vtNum1.end(), 0), vtNum1.end());
	HexToAscll(vtNum1);*/

	testTrans();
}






