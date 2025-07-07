#include <windows.h>
#include <dbt.h>  // 设备广播消息定义
#include <stdio.h>
#include <iostream>
#include <thread>
#include <usbiodef.h>
#include <initguid.h>
#include <devguid.h>
#include <functional>
#include <hidsdi.h>
#include <stdio.h>

#include "pch.h"
#include "test1.h"

#define _CONSOLE

#pragma comment(lib,"hid.lib")

using namespace std;

DEFINE_GUID(GUID_DEVINTERFACE_USB_DEVICE, 0xA5DCBF10L, 0x6530, 0x11D2, 0x90, 0x1F, 0x00, \
	0xC0, 0x4F, 0xB9, 0x51, 0xED);

// 全局变量
HWND g_hWnd = NULL;
MSG msg;
HDEVNOTIFY g_hDevNotify = NULL;
BOOL g_run = true;
char msginfo[40];

#ifdef _CONSOLE

LRESULT CALLBACK WndProc(HWND hwd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return 0;
}


void Func1(HWND hd)
{
	char c;
	while (g_run)
	{
		std::cin >> c;
		if (c == 'q')
		{
			g_run = false;
			PostMessage(hd, WM_DESTROY, NULL, NULL);
		}
	}
}


void GetDevVersion(LPCSTR _param)
{
	HANDLE hand = nullptr;
	USHORT ver = 0;

	hand = CreateFile(_param, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);

	HIDD_ATTRIBUTES hid_attributes = {0};
	hid_attributes.Size = sizeof(HIDD_ATTRIBUTES);

	if (hand == INVALID_HANDLE_VALUE)
	{
		cerr << "handle error " << endl;
		return;
	}

	if (HidD_GetAttributes(hand, &hid_attributes))
	{
		ver = hid_attributes.VersionNumber;
		//cout << ver << endl;
		printf("0x%x \n", ver);
	}

	CloseHandle(hand);
}

void GetDevGuid()
{
	GUID hidguid;
	HidD_GetHidGuid(&hidguid);

	HIDP_LINK_COLLECTION_NODE collection[10];

	std::wcout << hidguid.Data1 << static_cast < void*>(L"-") << hidguid.Data2 << static_cast < void*>(L"-") << hidguid.Data3 << L"-" << hidguid.Data4 << endl;	
}

void main()
{
	const char* _data = "\\\\?\\hid#vid_056e&pid_0194&mi_02#9&281097fd&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}";
	//GetDevVersion(_data);
	GetDevGuid();
	
}


#else 

void _Wnd::_OnCreate()
{
	HPOWERNOTIFY hpty = RegisterSuspendResumeNotification(g_hWnd, DEVICE_NOTIFY_WINDOW_HANDLE);
}

// 窗口过程
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	LONG_PTR ptr = ::GetWindowLongPtr(hwnd, GWL_STYLE);
	switch (msg) {
	case WM_CREATE:
		// 注册设备通知
		DEV_BROADCAST_DEVICEINTERFACE NotificationFilter;
		ZeroMemory(&NotificationFilter, sizeof(NotificationFilter));
		NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
		NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
		NotificationFilter.dbcc_classguid = GUID_DEVINTERFACE_USB_DEVICE;	//过滤usb设备

		g_hDevNotify = RegisterDeviceNotification(	//注册设备通知的函数
			hwnd,
			(PVOID)&NotificationFilter,
			DEVICE_NOTIFY_WINDOW_HANDLE
		);

		if (g_hDevNotify == NULL) {
			//printf("RegisterDeviceNotification failed: %d\n", GetLastError());
			sprintf_s(msginfo, "RegisterDeviceNotification failed: %d", GetLastError());
			MessageBox(hwnd, msginfo, NULL, MB_ICONERROR );
		}

		//_OnCreate();

		break;

	case WM_DEVICECHANGE:
		switch (wParam) {
		case DBT_DEVICEARRIVAL:
			//printf("设备已连接\n");
			sprintf_s(msginfo, "设备已连接\n");
			MessageBox(hwnd, msginfo, NULL, MB_ICONINFORMATION);
			break;
		case DBT_DEVICEREMOVECOMPLETE:
			//printf("设备已断开\n");
			sprintf_s(msginfo, "设备断开\n");
			MessageBox(hwnd, msginfo, NULL, MB_ICONINFORMATION);
			break;
		case DBT_DEVNODES_CHANGED:
			//printf("设备树发生变化\n");
			sprintf_s(msginfo, "设备树发生变化\n");
			MessageBox(hwnd, msginfo, NULL, MB_ICONINFORMATION);
			break;
		}
		break;

	case WM_DESTROY:
		// 取消设备通知
		if (g_hDevNotify != NULL) {
			UnregisterDeviceNotification(g_hDevNotify);
			g_hDevNotify = NULL;
		}
		//printf("销毁窗口 \n");
		sprintf_s(msginfo, "销毁窗口\n");
		MessageBox(hwnd, msginfo, NULL, MB_ICONINFORMATION);
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

void Func1(HWND hd, MSG ms)
{
	char c;
	while (g_run)
	{
		std::cin >> c;
		if (c == 'q')
		{
			g_run = false;
			PostMessage(hd, WM_DESTROY, NULL, NULL);
		}
	}
	while (PeekMessage(&ms, hd, 0, 0, PM_REMOVE))
	{
		switch (ms.message)
		{
		case WM_RBUTTONUP:
			sprintf_s(msginfo, "RBUTTONUP\n");
			MessageBox(hd, msginfo, NULL, MB_ICONINFORMATION);
			PostMessage(hd, WM_DESTROY, NULL, NULL);
			break;
		default:
			break;
		}
	}


}

// 主函数
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	// 注册窗口类

	_Wnd* wnd = new _Wnd();
 
	WNDCLASS wc = { 0 };
	//wc.lpfnWndProc = (WNDPROC)wnd->WndProc(g_hWnd, 0, NULL, NULL);
	wc.lpfnWndProc = (WNDPROC)WndProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszClassName = "DeviceNotifyWindow";

	if (!RegisterClass(&wc)) {
		return 0;
	}

	// 创建窗口
	g_hWnd = CreateWindow(
		"DeviceNotifyWindow",
		"设备通知示例",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 400, 300,
		NULL, NULL, hInstance, NULL
	);

	if (g_hWnd == NULL) {
		return 0;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);


	std::thread t1(std::bind(Func1, g_hWnd, msg));

	// 消息循环
	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	g_run = false;
	if (t1.joinable())
	{
		t1.join();
	}

	return 0;
}

#endif 