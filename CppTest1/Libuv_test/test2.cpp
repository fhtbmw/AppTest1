#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <uv.h>
#include "win/internal.h"
#include <memory>
#include <iostream>
#include <thread>
#include <functional>
#include <string>
#include <windows.h>
#include <condition_variable>

#define TEST_CASE 7
#define TEST_HOOK_LIBUV

using namespace std;

uv_loop_t* loop;
uv_process_t child_req;
uv_process_options_t options;

#if TEST_CASE == 1

void on_exit(uv_process_t* req, int64_t exit_status, int term_signal) {
	fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
	uv_close((uv_handle_t*)req, NULL);
}

int main() {
	loop = uv_default_loop();

	size_t size = 500;
	char path[500];
	uv_exepath(path, &size);
	//strcpy(path + (strlen(path) - strlen("proc-streams")), "test");

	char* args[2];
	args[0] = path;
	args[1] = NULL;

	/* ... */

	options.stdio_count = 3;
	uv_stdio_container_t child_stdio[3];
	child_stdio[0].flags = UV_IGNORE;
	child_stdio[1].flags = UV_IGNORE;
	child_stdio[2].flags = UV_INHERIT_FD;
	child_stdio[2].data.fd = 2;
	options.stdio = child_stdio;

	options.exit_cb = on_exit;
	options.file = args[0];
	options.args = args;

	int r;
	if ((r = uv_spawn(loop, &child_req, &options))) {
		fprintf(stderr, "%s\n", uv_strerror(r));
		return 1;
	}

	return uv_run(loop, UV_RUN_DEFAULT);
}

#elif TEST_CASE == 2

void cleanup_handles(uv_process_t* req, int64_t exit_status, int term_signal) {
	fprintf(stderr, "Process exited with status %" PRId64 ", signal %d\n", exit_status, term_signal);
	uv_close((uv_handle_t*)req->data, NULL);
	uv_close((uv_handle_t*)req, NULL);
}

void invoke_cgi_script(uv_tcp_t* client) {
	char path[500];
	size_t size = sizeof(path);
	uv_exepath(path, &size);
	strcpy(path + (strlen(path) - strlen("cgi")), "tick");

	char* args[2];
	args[0] = path;
	args[1] = NULL;

	/* ... finding the executable path and setting up arguments ... */

	options.stdio_count = 3;
	uv_stdio_container_t child_stdio[3];
	child_stdio[0].flags = UV_IGNORE;
	child_stdio[1].flags = UV_INHERIT_STREAM;
	child_stdio[1].data.stream = (uv_stream_t*)client;
	child_stdio[2].flags = UV_IGNORE;
	options.stdio = child_stdio;

	options.exit_cb = cleanup_handles;
	options.file = args[0];
	options.args = args;

	// Set this so we can close the socket after the child process exits.
	child_req.data = (void*)client;
	int r;
	if ((r = uv_spawn(loop, &child_req, &options))) {
		fprintf(stderr, "%s\n", uv_strerror(r));
		return;
	}
}

void alloc_buffer(uv_handle_t* handle,
	size_t suggested_size,
	uv_buf_t* buf)
{
	buf->base = (char*)malloc(suggested_size);
	buf->len = suggested_size;
}

void echo_read(uv_stream_t* stream,
	ssize_t nread,
	const uv_buf_t* buf)
{
	printf("read data: %s \n", buf->base);
}

void write_func(uv_write_t* req, int status)
{

}

void on_new_connection(uv_stream_t* server, int status) {
	if (status == -1) {
		// error!
		return;
	}

	uv_write_t uvt;

	if (server->type == UV_TCP)
	{
		uv_tcp_t* client = (uv_tcp_t*)malloc(sizeof(uv_tcp_t));
		uv_tcp_init(loop, client);
		if (uv_accept(server, (uv_stream_t*)client) == 0) {
			invoke_cgi_script(client);
		}
		else {
			uv_close((uv_handle_t*)client, NULL);
		}
	}
	else if (server->type == UV_NAMED_PIPE)
	{
		uv_pipe_t* client = (uv_pipe_t*)malloc(sizeof(uv_pipe_t));

		char cBuf[64] = "server send : dsadasadasdxzccsxcs";
		DWORD dwWrittenSize = 0;

		uv_buf_t WriteBuf;
		WriteBuf.base = (char*)malloc(sizeof(uv_buf_t));
		WriteBuf.len = 128;

		uv_pipe_init(loop, client, 0);
		if (uv_accept(server, (uv_stream_t*)client) == 0)
		{
			uv_read_start((uv_stream_t*)client, alloc_buffer, echo_read);
			assert(client != NULL);
			if (!WriteFile(client->handle, cBuf, strlen(cBuf), &dwWrittenSize, NULL))
			{
				printf("server WriteFile failed error code: %d", GetLastError());
			}
			/*int ret = uv_write(&uvt, (uv_stream_t*)client, &WriteBuf, 1, NULL);
			if (ret)
			{
				printf("uv_pipe_write error: %s \n", uv_err_name(ret));
			}*/

		}
		else
		{
			uv_close((uv_handle_t*)client, NULL);
		}
	}

}

int test_tcp() {
	loop = uv_default_loop();

	uv_tcp_t server;
	uv_tcp_init(loop, &server);

	struct sockaddr_in bind_addr;
	uv_ip4_addr("0.0.0.0", 7000, &bind_addr);
	uv_tcp_bind(&server, (const struct sockaddr*)&bind_addr, 0);
	int r = uv_listen((uv_stream_t*)&server, 128, on_new_connection);
	if (r) {
		fprintf(stderr, "Listen error %s\n", uv_err_name(r));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}

int test_pipe()
{
	loop = uv_default_loop();

	uv_pipe_t _pipe;
	uv_pipe_init(loop, &_pipe, 0);
	signal;

	int r = uv_pipe_bind(&_pipe, "\\\\.\\pipe\\MyPipe");
	if (r)
	{
		printf("Bind error %s \n", uv_err_name(r));
	}

	int ret = uv_listen((uv_stream_t*)&_pipe, 128, on_new_connection);
	if (ret)
	{
		printf("listen error %s \n", uv_err_name(ret));
		return 1;
	}
	return uv_run(loop, UV_RUN_DEFAULT);
}

void main()
{
	test_pipe();
	//test_tcp();
}

////do nothing
//void main()
//{
//	uv_loop_t* uvloop = uv_default_loop();
//	uv_run(uvloop, UV_RUN_DEFAULT);
//	uv_loop_close(uvloop);
//}

#elif TEST_CASE == 3

void test_detach()
{
	loop = uv_default_loop();

	char* cbuf[3];
	char cpath[100] = "C:\\Users\\admin\\source\\repos\\CppTest1\\x64\\Debug\\CppTest1.exe ", cparam[32] = "";
	size_t ssize = sizeof(cpath);
	//uv_exepath(cpath, &ssize);
	cbuf[0] = cpath;
	cbuf[1] = cparam;
	cbuf[2] = NULL;

	options.exit_cb = NULL;
	options.file = cbuf[0];
	options.args = cbuf;

	int r;
	if (r = uv_spawn(loop, &child_req, &options))
		printf("%s \n", uv_strerror(r));
	return;

	fprintf(stderr, "Launched sleep with PID %d\n", child_req.pid);
	uv_unref((uv_handle_t*)&child_req);
	uv_run(loop, UV_RUN_DEFAULT);
}

void main()
{
	test_detach();
}

#elif TEST_CASE == 4
#include <stdio.h>
#include <uv.h>

int64_t counter = 0;

void wait_for_a_while(uv_idle_t* handle) {
	counter++;

	if (counter >= 10e6)
		uv_idle_stop(handle);
}

int main() {
	uv_idle_t idler;

	uv_idle_init(uv_default_loop(), &idler);
	uv_idle_start(&idler, wait_for_a_while);

	printf("Idling...\n");
	uv_run(uv_default_loop(), UV_RUN_DEFAULT);

	uv_loop_close(uv_default_loop());
	return 0;
}

#elif TEST_CASE == 5
uv_fs_t stdin_watcher;
uv_idle_t idler;
char buffer[1024];

void crunch_away(uv_idle_t* handle) {
	// Compute extra-terrestrial life
	// fold proteins
	// computer another digit of PI
	// or similar
	fprintf(stderr, "Computing PI...\n");
	// just to avoid overwhelming your terminal emulator
	uv_idle_stop(handle);
}

void on_type(uv_fs_t* req) {
	if (stdin_watcher.result > 0) {
		buffer[stdin_watcher.result] = '\0';
		printf("Typed %s\n", buffer);

		uv_buf_t buf = uv_buf_init(buffer, 1024);
		uv_fs_read(loop, &stdin_watcher, 0, &buf, 1, -1, on_type);
		uv_idle_start(&idler, crunch_away);
	}
	else if (stdin_watcher.result < 0) {
		fprintf(stderr, "error opening file: %s\n", uv_strerror(req->result));
	}
}

int test1() {
	loop = uv_default_loop();

	uv_idle_init(loop, &idler);

	uv_buf_t buf = uv_buf_init(buffer, 1024);
	uv_fs_read(loop, &stdin_watcher, 0, &buf, 1, -1, on_type);
	uv_idle_start(&idler, crunch_away);
	return uv_run(loop, UV_RUN_DEFAULT);
}


class TT
{
public:
	void show()
	{

	}
};

void test2()
{
	wchar_t username[MAX_PATH] = { 0 };
	DWORD userlength = MAX_PATH;
	GetUserNameW(username, &userlength);

	std::unique_ptr<TT> up1 = std::make_unique<TT>(2);
	std::shared_ptr<TT> sp1 = std::make_shared<TT>(3);

	sp1->show();
	up1->show();

}

#elif TEST_CASE == 6

//thread signal

#define SIGUSR1 9
#define _Msg 2

uv_loop_t* create_loop()
{
	uv_loop_t* loop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	if (loop) {
		uv_loop_init(loop);
	}
	return loop;
}

void signal_handler(uv_signal_t* handle, int signum)
{
	printf("Signal received: %d\n", signum);
	uv_signal_stop(handle);
}

void threadfunc(void* param)
{
	//PostMessage(HWND_BROADCAST, _Msg, NULL, NULL); // 广播Msg，若系统其他程序无回应，会出错



}

// two signal handlers in one loop
void thread1_worker(void* userp)
{
	uv_loop_t* loop1 = create_loop();

	uv_signal_t sig1a, sig1b;

#ifdef _WIN32
	uv_signal_init(loop1, &sig1a);
	uv_signal_start(&sig1a, signal_handler, _Msg);

#elif
	uv_signal_init(loop1, &sig1a);
	uv_signal_start(&sig1a, signal_handler, SIGUSR1);
	uv_signal_init(loop1, &sig1b);
	uv_signal_start(&sig1b, signal_handler, SIGUSR1);

#endif
	uv_run(loop1, UV_RUN_DEFAULT);

}

// two signal handlers, each in its own loop
void thread2_worker(void* userp)
{
	uv_loop_t* loop2 = create_loop();
	uv_loop_t* loop3 = create_loop();

	uv_signal_t sig2;
	uv_signal_init(loop2, &sig2);
	uv_signal_start(&sig2, signal_handler, SIGUSR1);

	uv_signal_t sig3;
	uv_signal_init(loop3, &sig3);
	uv_signal_start(&sig3, signal_handler, SIGUSR1);

	while (uv_run(loop2, UV_RUN_NOWAIT) || uv_run(loop3, UV_RUN_NOWAIT)) {
	}
}

void test3()
{
	printf("PID %d\n", getpid());

	uv_thread_t thread1, thread2;

	uv_thread_create(&thread1, thread1_worker, 0);
	uv_thread_create(&thread2, threadfunc, 0);

	uv_thread_join(&thread1);
	uv_thread_join(&thread2);
	return;
}

#elif TEST_CASE == 7

typedef void* QU[2];

#define Q_N(q) (*(QU**) &((*(q))[0]))	//            *(QU **) & QU[0]	-> QU*

struct _SS
{
	string name;
	int a[3];
	uint32_t uinb;
};

template<typename ...Args>
class Ctest : public Args ...
{

};

void test4()
{
	/*QU _q1;
	Q_N(&_q1);

	int a = 1;
	*(char*)(&a) = 2;*/
	/*Ctest<_SS> ct1;

	_SS s1;
	cout << offsetof(_SS, uinb) << endl;

	string _s("s");
	cout << sizeof(_s) << " " << _s.size() << " " << _s.length() << endl;*/

	uv_loop_t* uvl = uv_default_loop();

	uv_loop_init(uvl);

	uv_run(uvl, UV_RUN_DEFAULT);

	uv_loop_close(uvl);

}

#include <iostream>
#include <thread>
#include <Windows.h>
#include <mutex>
#include <memory>

#define WM_USER1 (WM_USER + 1)
#define WM_USER2 (WM_USER + 2)

HWND hwnd;
HANDLE hevent;
DWORD dwWait;
mutex _mu;

int g_itimer = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_TIMER:
		if (wParam == 1)  // 检查定时器ID
		{
			std::lock_guard<mutex> lg(_mu);
			g_itimer++;
			InvalidateRect(hwnd, NULL, TRUE);  // 触发重绘
		}
		break;

	case WM_USER2:
		PostQuitMessage(0);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_PAINT:
	{
		std::lock_guard<mutex> lg(_mu);
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// 清除背景
		RECT rect;
		GetClientRect(hwnd, &rect);
		FillRect(hdc, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

		// 绘制当前计时器值
		std::string st = std::to_string(g_itimer);

		if (g_itimer % 10 == 0)
		{
			SetTextColor(hdc, RGB(45, 25, 200));
		}
		else 
		{
			SetTextColor(hdc, RGB(0, 0, 0));
		}
		SetBkMode(hdc, TRANSPARENT);
		TextOutA(hdc, 20, 20, st.c_str(), st.length());

		EndPaint(hwnd, &ps);
	}
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

HWND CreateMyWindow()
{
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"MyWindowClass";

	RegisterClass(&wc);

	HWND hwnd = CreateWindow(
		L"MyWindowClass",
		L"Message Test Window",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
		NULL, NULL, GetModuleHandle(NULL), NULL);

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	return hwnd;
}

uv_loop_t* CreateMyLoop()
{
	uv_loop_t* uvloop = (uv_loop_t*)malloc(sizeof(uv_loop_t));
	if (uvloop)
	{
		uv_loop_init(uvloop);
	}
	return uvloop;
}

void TimerCallBack(HWND hwnd, UINT uint, UINT_PTR up, DWORD dw)
{

	PostMessage(hwnd, WM_USER1, 0, 0);
	InvalidateRect(hwnd, NULL, TRUE);
}

void ThreadFunc2(void* args)
{
	MSG msg;
	BOOL bret;

	hwnd = CreateMyWindow();  // 创建窗口

	assert(hwnd != NULL);

	if (hevent != NULL)
	{
		if (SetEvent(hevent) == NULL)
		{
			cout << "SetEvent faield errno: " << GetLastError() << endl;
		}
	}

	SetTimer(hwnd, 1, 1000, NULL);

	while ((bret = GetMessage(&msg, NULL, 0, 0)) != 0)  // 监听所有消息
	{

		if (bret == -1)
		{
			std::cerr << "GetMessage failed errorno: " << GetLastError() << std::endl;
			return;
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);  // 分发消息到窗口过程
		}
	}

	KillTimer(hwnd, 1);

	PostMessage(hwnd, WM_USER2, NULL, 0);

	cout << "GetMsg 0 \n";
}

void test6()
{
	uv_thread_t t1, t2;

	uv_thread_create(&t2, ThreadFunc2, 0);

	uv_thread_join(&t2);
}

#ifdef TEST_HOOK_LIBUV

HHOOK g_kbhook;
HHOOK g_mghook;
std::atomic_bool g_bflag;

LRESULT CALLBACK KeyBoardCall(int code, WPARAM wParam, LPARAM lParam)
{
	if (code == HC_ACTION)
	{
		KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;
		MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;

		if (wParam == WM_KEYDOWN)
		{
			cout << "key pressed: " << kb->vkCode << endl;
		}
		else if (wParam == WM_LBUTTONDOWN)
		{
			cout << "L Mouse Button pressed: " << endl;
		}
		else if (wParam == WM_RBUTTONDOWN)
		{
			cout << "R Mouse Button pressed: " << endl;
		}
		else if (wParam == WM_MBUTTONDOWN)
		{
			cout << "M Mouse Button pressed: " << endl;
		}
	}
	return CallNextHookEx(g_kbhook, code, wParam, lParam);
}

LRESULT CALLBACK MouseCall(int code, WPARAM wParam, LPARAM lParam)
{
	if (code == HC_ACTION)
	{
		MSLLHOOKSTRUCT* ms = (MSLLHOOKSTRUCT*)lParam;

		if (wParam == WM_LBUTTONDOWN)
		{
			cout << "L Mouse Button pressed: " << endl;
		}
		else if (wParam == WM_RBUTTONDOWN)
		{
			cout << "R Mouse Button pressed: " << endl;
		}
		else if (wParam == WM_MBUTTONDOWN)
		{
			cout << "M Mouse Button pressed: " << endl;
		}
	}
	return CallNextHookEx(g_kbhook, code, wParam, lParam);
}


void ThreadFuncGetKB()
{
	g_kbhook =  SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardCall, NULL, 0);
	if (g_kbhook == NULL)
	{
		cerr << "HookKB failed error no: " << GetLastError() << endl;
		return;
	}

	MSG	msg;
	while (GetMessage(&msg, NULL, 0, 0) && !g_bflag)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(g_kbhook);
}

void ThreadFunGetMS()
{
	g_mghook = SetWindowsHookEx(WH_MOUSE_LL, MouseCall, NULL, 0);
	if (g_mghook == NULL)
	{
		cerr << "Hook Ms error no: " << GetLastError() << endl;
		return;
	}

	MSG	msg;
	while (GetMessage(&msg, NULL, 0, 0) && !g_bflag)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(g_mghook);
}

BOOL WINAPI ConsoleCtrlHandler(DWORD dwAction)
{
	if (dwAction == CTRL_C_EVENT)
	{
		g_bflag == TRUE;
		return TRUE;
	}
	return FALSE;
}

void uv_thrad_func(void* args)
{
	ThreadFuncGetKB();
}

void uv_thread_func2(void* args)
{
	ThreadFunGetMS();
}

void test7()
{
	uv_thread_t t, tt;
	
	SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE);

	uv_thread_create(&t, uv_thrad_func, NULL);
	uv_thread_create(&tt, uv_thread_func2, NULL);

	uv_thread_join(&t);
	uv_thread_join(&tt);
}

#endif

#endif

int main()
{
	test6();
}
