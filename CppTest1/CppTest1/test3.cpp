#include <windows.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <tchar.h>
#include "pch.h"

#define  XX  4

using namespace std;

#if XX == 1

void WriteSharedMemory()
{
	HANDLE hFile = CreateFile(
		"文本.txt",							// 文件名
		GENERIC_READ | GENERIC_WRITE,		// 所需访问权限
		0,									// 共享模式
		NULL,								// 安全属性
		OPEN_EXISTING,						// 创建处置
		FILE_ATTRIBUTE_NORMAL,				// 文件属性
		NULL								// 模板文件
	);

	if (hFile == INVALID_HANDLE_VALUE) {
		std::cerr << "Failed to open file." << std::endl;
		return;
	}

	DWORD dfileattri = GetFileAttributes("文本.txt");
	cout << dfileattri << endl;

	// 创建文件映射对象
	HANDLE hMapping = CreateFileMapping(
		hFile,                      // 文件句柄
		NULL,                       // 安全属性
		PAGE_READWRITE,             // 保护标志
		0,                          // 最大大小高位
		0,                          // 最大大小低位
		"Share Memory1"              // 映射名称
	);

	if (hMapping == NULL) {
		std::cerr << "Failed to create file mapping error no: " << GetLastError() << std::endl;
		CloseHandle(hFile);
		return;
	}

	// 从文件的第 1000 字节开始映射 4096 字节
	DWORD dwOffsetHigh = 0; // 高 32 位偏移量
	DWORD dwOffsetLow = 0; // 低 32 位偏移量
	SIZE_T dwBytesToMap = 4096; // 映射的字节数

	LPVOID pMapView = MapViewOfFile(
		hMapping,                   // 映射句柄
		FILE_MAP_READ | FILE_MAP_WRITE, // 期望访问权限
		dwOffsetHigh,               // 文件偏移量高位
		dwOffsetLow,                // 文件偏移量低位
		dwBytesToMap                // 要映射的字节数
	);

	if (pMapView == NULL) {
		std::cerr << "Failed to map view of file. " << GetLastError() << std::endl;
		CloseHandle(hMapping);
		CloseHandle(hFile);
		return;
	}

	// 读取或写入映射的视图
	// 例如，将数据写入映射区域
	strcpy_s(static_cast<char*>(pMapView), dwBytesToMap, "Data written to file!");

	std::cout << "File successfully mapped and modified." << std::endl;

	std::string c;
	getline(std::cin, c);

	// 清理资源
	UnmapViewOfFile(pMapView);
	CloseHandle(hMapping);
	CloseHandle(hFile);

}

void testSharedMemory()
{
	HANDLE handle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, "Share Memory");
	if (handle == INVALID_HANDLE_VALUE)
	{
		cerr << "OpenFileMapping faied error no: %d" << GetLastError() << endl;
		CloseHandle(handle);
		return;
	}

	char buf[512];

	LPVOID lpres = MapViewOfFile(handle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 512);
	if (lpres == NULL)
	{
		cerr << "MapViewOfFile failed error no: %d" << GetLastError() << endl;
		CloseHandle(handle);
		return;
	}

	strcpy_s(buf, 512, static_cast<char*>(lpres));
	cout << "read data : " << buf << endl;

	UnmapViewOfFile(lpres);
	CloseHandle(handle);

}

void main()
{
	//testSharedMemory();
	WriteSharedMemory();
}

#elif XX == 2

#define ThreadNumbers 2

HANDLE whandle;
HANDLE rhandle;
HANDLE g_hthreads[ThreadNumbers];

HANDLE g_handle;

void _SetEvent(HANDLE handle)
{
	SetEvent(handle);
}

DWORD WINAPI WriteThreadFunc(LPVOID param)
{
	HANDLE fhandle = CreateFile("D:\\t.txt", GENERIC_READ | GENERIC_WRITE, FILE_MAP_READ | FILE_MAP_WRITE, 0, OPEN_EXISTING,	
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (fhandle == INVALID_HANDLE_VALUE)
	{
		cerr << "CreateFile failed error no: " << GetLastError() << endl;
		CloseHandle(fhandle);
		return -1;
	}

	if (!GetFileAttributes("D:\\t.txt"))
	{
		cerr << "File not exists \n" << GetLastError() << endl;
		CloseHandle(fhandle);
		return -1;
	}

	HANDLE fmap = CreateFileMapping(fhandle, 0, PAGE_READWRITE, 0, 0, "sharememory");
	if (fmap == NULL)
	{
		cerr << "CreateFileMapping failed error no: " << GetLastError() << endl;
		CloseHandle(fhandle);
		return -1;
	}

	assert(fmap);
	LPVOID lpres = MapViewOfFile(fmap, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
	if (lpres == NULL)
	{
		cerr << "MapViewOfFile failed error no:" << GetLastError() << endl;
		CloseHandle(fhandle);
		CloseHandle(fmap);
		return -1;
	}

	strcpy_s(static_cast<char*>(lpres), 512, "write data to file");

	_SetEvent(whandle);

	DWORD ret = WaitForSingleObject(rhandle, INFINITE);

	switch (ret)
	{
	case WAIT_OBJECT_0:
		cout << "wait for read thread success" << endl;
		UnmapViewOfFile(lpres);
		CloseHandle(fhandle);
		CloseHandle(fmap);
		ResetEvent(rhandle);
		break;
	default:
		cout << "wait for read thread success failed error no: " << GetLastError() << endl;
		break;
	}
	return 0;
}

DWORD WINAPI ReadThreadFunc(LPVOID param)
{
	DWORD ret = WaitForSingleObject(whandle, INFINITE);

	switch (ret)
	{
	case WAIT_OBJECT_0:

	{
		g_handle = OpenFileMapping(FILE_MAP_READ | FILE_MAP_WRITE, TRUE, "sharememory");
		if (g_handle == NULL)
		{
			cerr << "OpenFileMapping failed error no: " << GetLastError() << endl;
			return -1;
		}

		char readbuf[512];
		LPVOID lpres = MapViewOfFile(g_handle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 512);
		if (lpres == NULL)
		{
			cerr << "MapViewOfFile failed error no: " << GetLastError() << endl;
			CloseHandle(g_handle);
			return -1;
		}

		strcpy_s(readbuf, 512, static_cast<char*>(lpres));
		cout << "read data: " << readbuf << endl;
		ResetEvent(whandle);
		_SetEvent(rhandle);
		UnmapViewOfFile(lpres);
		CloseHandle(g_handle);
	}
		break;

	default:
		cout << "wait for write thread failed " << endl;
		CloseHandle(g_handle);
		break;
	}
	return 0;
}

void _CreateThreads()
{
	g_hthreads[0] = CreateThread(NULL, 0, WriteThreadFunc, nullptr, 0, nullptr);
	g_hthreads[1] = CreateThread(NULL, 0, ReadThreadFunc, nullptr, 0, nullptr);

}

void test1()
{
	_CreateThreads();
	DWORD ret = WaitForMultipleObjects(ThreadNumbers, g_hthreads, 1, INFINITE);
	if (ret == WAIT_OBJECT_0)
	{
		cerr << "threads endded" << endl;
	}
	else {
		cerr << "waitformultipleobjeects failed error no :" << GetLastError() << endl;
	}
	CloseHandle(rhandle);
	CloseHandle(whandle);
}

void test2()
{
	_CreateThreads();
	DWORD ret = WaitForSingleObject(g_hthreads[0], INFINITE);
	if (ret == WAIT_OBJECT_0)
	{
		cerr << "threads endded" << endl;
	}
	else {
		cerr << "wait failed error no :" << GetLastError() << endl;
	}
	CloseHandle(rhandle);
	CloseHandle(whandle);
}

void main()
{
	//CreatePipe();
	test1();
}

#elif XX == 3

//Pipe ChildProcess
#include<windows.h>
#include <iostream>

#define BUFSIZE 4096

using namespace std;

void test1()
{
	CHAR cbBuf [BUFSIZE];
	DWORD dwRead, dwWritten;
	HANDLE hStdin, hStdOut;
	BOOL bSuccess;

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	hStdin = GetStdHandle(STD_INPUT_HANDLE);

	if (hStdOut == INVALID_HANDLE_VALUE
		|| hStdin == INVALID_HANDLE_VALUE)
	{
		cerr << "GetStdHandle error " << GetLastError() << endl;
		return;
	}

	for (;;)
	{
		bSuccess = ReadFile(hStdin, cbBuf, BUFSIZE, &dwRead, NULL);
		if (!bSuccess || dwRead == 0)
		{
			break;
		}

		bSuccess = WriteFile(hStdOut, cbBuf, dwRead, &dwWritten, NULL);
		if (!bSuccess)
		{
			break;
		}

	}
	return;
}

void test2()
{
	HANDLE handle = GetStdHandle(STD_INPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	char cBuf[8];

	while (true)
	{
		cout << " input: \n";
		cin >> cBuf;

		cci.dwSize = 1;
		if (!strcmp(cBuf, "on"))
		{
			cci.bVisible = 1;
			SetConsoleCursorInfo(handle, &cci);
		}
		else if (!strcmp(cBuf, "off"))
		{
			cci.bVisible = 0;
			SetConsoleCursorInfo(handle, &cci);
		}
		else
		{
			cerr << " input error \n";
			break;
		}
	}
}

void main()
{
	test1();
}

#elif XX == 4

//namepipe client

#include <windows.h> 
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

#define BUFSIZE 512

int _tmain(int argc, TCHAR* argv[])
{
	HANDLE hPipe;
	LPTSTR lpvMessage = (LPTSTR)"Default message from client.";
	TCHAR  chBuf[BUFSIZE];
	BOOL   fSuccess = FALSE;
	DWORD  cbRead, cbToWrite, cbWritten, dwMode;
	//LPTSTR lpszPipename = TEXT("\\\\.\\pipe\\mynamedpipe");
	LPTSTR lpszPipename = (LPTSTR)"\\\\.\\pipe\\fir";

	if (argc > 1)
		lpvMessage = argv[1];

	// Try to open a named pipe; wait for it, if necessary. 

	while (1)
	{
		hPipe = CreateFile(
			lpszPipename,   // pipe name 
			GENERIC_WRITE |
			GENERIC_READ,  // read and write access 
			0,              // no sharing 
			NULL,           // default security attributes
			OPEN_EXISTING,  // opens existing pipe 
			0,              // default attributes 
			NULL);          // no template file 

		// Break if the pipe handle is valid. 

		DWORD ret = GetLastError();

		if (hPipe != INVALID_HANDLE_VALUE)
			break;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() != ERROR_PIPE_BUSY)
		{
			_tprintf(TEXT("Could not open pipe. GLE=%d\n"), GetLastError());
			return -1;
		}

		// All pipe instances are busy, so wait for 20 seconds. 

		if (!WaitNamedPipe(lpszPipename, 20000))
		{
			printf("Could not open pipe: 20 second wait timed out.");
			return -1;
		}
	}

	// The pipe connected; change to message-read mode. 

	dwMode = PIPE_READMODE_MESSAGE;
	fSuccess = SetNamedPipeHandleState(
		hPipe,    // pipe handle 
		&dwMode,  // new pipe mode 
		NULL,     // don't set maximum bytes 
		NULL);    // don't set maximum time 
	if (!fSuccess)
	{
		_tprintf(TEXT("SetNamedPipeHandleState failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	// Send a message to the pipe server. 

	cbToWrite = (lstrlen(lpvMessage) + 1) * sizeof(TCHAR);
	_tprintf(TEXT("Sending %d byte message: \"%s\"\n"), cbToWrite, lpvMessage);

	fSuccess = WriteFile(
		hPipe,                  // pipe handle 
		lpvMessage,             // message 
		cbToWrite,              // message length 
		&cbWritten,             // bytes written 
		NULL);                  // not overlapped 

	if (!fSuccess)
	{
		_tprintf(TEXT("WriteFile to pipe failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	printf("\nMessage sent to server, receiving reply as follows:\n");

	do
	{
		// Read from the pipe server. 

		fSuccess = ReadFile(
			hPipe,    // pipe handle 
			chBuf,    // buffer to receive reply 
			BUFSIZE * sizeof(TCHAR),  // size of buffer 
			&cbRead,  // number of bytes read 
			NULL);    // not overlapped 

		if (!fSuccess )
			break;

		_tprintf(TEXT("\"%s\"\n"), chBuf);
	} while (!fSuccess);  // repeat loop if ERROR_MORE_DATA 

	if (!fSuccess)
	{
		_tprintf(TEXT("ReadFile from pipe failed. GLE=%d\n"), GetLastError());
		return -1;
	}

	printf("\n<End of message, press ENTER to terminate connection and exit>");
	_getch();

	CloseHandle(hPipe);

	return 0;
}

#elif XX ==5

#include <windows.h>
#include <iostream>

int main() {
	// 1. 连接到命名管道

	//LPTSTR lpszpipename = LPTSTR("\\\\.\\pipe\\MyNamedPipe");
	LPTSTR lpszpipename = LPTSTR("\\\\.\\pipe\\fir");

	HANDLE hPipe = CreateFile(
		lpszpipename,  // 管道名称（必须与服务器一致）
		GENERIC_READ | GENERIC_WRITE, // 读写权限
		0,                            // 不共享
		NULL,                         // 默认安全属性
		OPEN_EXISTING,                // 打开已存在的管道
		0,                            // 默认属性
		NULL                          // 无模板文件
	);

	if (hPipe == INVALID_HANDLE_VALUE) {
		std::cerr << "CreateFile failed! Error: " << GetLastError() << std::endl;
		return 1;
	}

	std::cout << "Connected to server!" << std::endl;

	// 2. 发送消息给服务器
	const char* message = "Hello from client!";
	DWORD bytesWritten;
	if (!WriteFile(hPipe, message, strlen(message), &bytesWritten, NULL)) {
		std::cerr << "WriteFile failed! Error: " << GetLastError() << std::endl;
		CloseHandle(hPipe);
		return 1;
	}

	std::cout << "Message sent to server." << std::endl;

	// 3. 读取服务器响应
	char buffer[512];
	DWORD bytesRead;
	if (!ReadFile(hPipe, buffer, sizeof(buffer), &bytesRead, NULL)) {
		std::cerr << "ReadFile failed! Error: " << GetLastError() << std::endl;
		CloseHandle(hPipe);
		return 1;
	}

	assert(bytesRead < sizeof(buffer) - 1);
	buffer[bytesRead] = '\0';  // 确保字符串终止
	std::cout << "Received from server: " << buffer << std::endl;

	// 4. 关闭管道
	CloseHandle(hPipe);
	return 0;
}


#endif













