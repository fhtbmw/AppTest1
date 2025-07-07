#pragma once
#include <windows.h>

#include "pch.h"

class _Wnd
{
public:

	void _OnCreate();

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND _wnd;

};
