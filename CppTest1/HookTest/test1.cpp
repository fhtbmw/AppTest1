#include <windows.h>
#include <iostream>
#include <assert.h>
#include <thread>
#include <functional>

#include "ThreadWrapper.h"

#define TEST_KEYBOARD_HOOK

using namespace std;

#ifdef TEST_KEYBOARD_HOOK

HHOOK g_kbhook;
HHOOK g_mghook;

LRESULT CALLBACK KeyBoardProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code == HC_ACTION)
	{
		KBDLLHOOKSTRUCT* pkeyinfo = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_KEYDOWN || wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN 
			|| wParam == WM_MBUTTONDOWN)
		{
			std::cout << "key pressed: " << pkeyinfo->vkCode << std::endl;
		}
	}
	return CallNextHookEx(g_kbhook, code, wParam, lParam);
}

LRESULT CALLBACK MouseProc(int code, WPARAM wParam, LPARAM lParam)
{
	if (code == HC_ACTION)
	{
		MOUSEHOOKSTRUCT* mouseinfo = (MOUSEHOOKSTRUCT*)lParam;
		KBDLLHOOKSTRUCT* pkeyinfo = (KBDLLHOOKSTRUCT*)lParam;
		if (wParam == WM_MOUSEMOVE)
		{
			std::cout << "mouse move: " << mouseinfo->pt.x << " " << mouseinfo->pt.y << std::endl;
		}
		else if (wParam == WM_LBUTTONDOWN || wParam == WM_RBUTTONDOWN
			|| wParam == WM_MBUTTONDOWN)
		{
			std::cout << "mouse pressed: " <<  pkeyinfo->vkCode<< std::endl;
		}
	}
	return CallNextHookEx(g_kbhook, code, wParam, lParam);
}

void hookfunc1()
{
	g_kbhook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyBoardProc, NULL, 0);
	if (g_kbhook == NULL)
	{
		cerr << "hook keyboard failed error no : " << GetLastError() << endl;
	}

	MSG _msg;
	while (GetMessage(&_msg, NULL, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}

	if (g_kbhook)
	{
		UnhookWindowsHookEx(g_kbhook);
	}
}

void hookfunc2()
{
	g_mghook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, NULL, 0);
	if (g_mghook == NULL)
	{
		cerr << "hook keyboard failed error no : " << GetLastError() << endl;
	}

	MSG _msg;
	while (GetMessage(&_msg, NULL, 0, 0))
	{
		TranslateMessage(&_msg);
		DispatchMessage(&_msg);
	}

	if (g_mghook)
	{
		UnhookWindowsHookEx(g_mghook);
	}
}

void test1()
{
	_thread t1(hookfunc1);
	_thread t2(hookfunc2);
	
}

#endif


void main()
{
	test1();
}