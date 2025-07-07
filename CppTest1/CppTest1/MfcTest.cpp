#include<windows.h>
#include<iostream>

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

HANDLE _CreateWindow(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd) {
    WNDCLASS wc = { 0 };
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
    wc.hCursor = NULL;
    wc.hIcon = NULL;
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"Main";
    wc.lpszMenuName = L"m";
    RegisterClass(&wc);
    HWND hwd = CreateWindow(L"Edit", L"m", WS_OVERLAPPEDWINDOW, 300, 400, 290, 200, NULL, NULL, hInstance, NULL);
    ShowWindow(hwd, SW_SHOWNORMAL);
    UpdateWindow(hwd);
    return hwd;
}

int WinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nShowCmd)
{

    HANDLE hd=_CreateWindow(hInstance,hPrevInstance,lpCmdLine,nShowCmd);
    WaitForSingleObject(hd,INFINITE);
    
    return 0;
}