#include <windows.h>
#include <SetupAPI.h>
#include <hidsdi.h>
#include <iostream>
#include <tchar.h>
#include <hidclass.h>
#include <assert.h>
#include <string>
//#include <ntddk.h>
//#include><hidport.h>

#include "pch.h"
#include "hidapi.h"

#pragma comment(lib, "Setupapi.lib")
#pragma comment(lib, "hid.lib")

#define  XX 2

typedef int (*func1)(int, int);

using namespace std;

#if XX == 1

//枚举系统中HID
void EnumHidDevice()
{
	GUID hidguid;
	HidD_GetHidGuid(&hidguid);
	HDEVINFO devinfo = SetupDiGetClassDevs(&hidguid, NULL, NULL, DIGCF_PRESENT | DIGCF_DEVICEINTERFACE);		
	if (devinfo == INVALID_HANDLE_VALUE)
	{
		cerr << "SetupDiGetClassDevs failed error no: " << GetLastError() << endl;
		return;
	}

	SP_DEVICE_INTERFACE_DATA spdevinterdata;
	ZeroMemory(&spdevinterdata, sizeof(SP_DEVICE_INTERFACE_DATA));
	spdevinterdata.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);	//先置0后赋值

	DWORD dsize;

	for (int i =0;SetupDiEnumDeviceInterfaces(devinfo, NULL, &hidguid, i, &spdevinterdata); i++)
	{
		SetupDiGetDeviceInterfaceDetail(devinfo, &spdevinterdata, NULL, 0, &dsize, NULL);

		PSP_INTERFACE_DEVICE_DETAIL_DATA pspinterdevdetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(dsize);
		if (pspinterdevdetail)
		{
			pspinterdevdetail->cbSize = sizeof(PSP_INTERFACE_DEVICE_DETAIL_DATA);
		}

		SetupDiGetDeviceInterfaceDetail(devinfo, &spdevinterdata, pspinterdevdetail, dsize, NULL, NULL);	
		
		assert(pspinterdevdetail);
		_tprintf(TEXT("dev path: %s \n"), pspinterdevdetail->DevicePath);

		HANDLE devhandle;
		devhandle = CreateFile(pspinterdevdetail->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
			0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);

		if (devhandle == INVALID_HANDLE_VALUE)
		{
			cerr << " CreateFile failed error no: " << GetLastError() << endl;
			return;
		}

		HIDD_ATTRIBUTES hidattributes;
		ZeroMemory(&hidattributes, sizeof(hidattributes));


		assert(devhandle);
		HidD_GetAttributes(devhandle, &hidattributes);

		ULONG ulBufSize = 32;
		TCHAR tcBUf[32] = { 0 };
		
		assert(devhandle);
		if (HidD_GetProductString(devhandle, tcBUf, ulBufSize));

		PHIDP_PREPARSED_DATA phppreparseddata;
		HidD_GetPreparsedData(devhandle, &phppreparseddata);

		HIDP_CAPS hipdcaps;
		HidP_GetCaps(phppreparseddata, &hipdcaps);

		
	}

}



void main()
{

	EnumHidDevice();

	return ;
}

#elif XX == 2

void GetHidDeviceInfo(LPCSTR wcStrDevPath)
{
	HANDLE hHidDev;
	hHidDev = CreateFile(wcStrDevPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (hHidDev == INVALID_HANDLE_VALUE)
	{
		cerr << "CreateFile failed errorno:" << GetLastError() << endl;
		GUID_DEVINTERFACE_HID;
		return;
	}

	HIDD_ATTRIBUTES hidattr;
	ZeroMemory(&hidattr, sizeof(hidattr));
	hidattr.Size = sizeof(HIDD_ATTRIBUTES);

	if (!HidD_GetAttributes(hHidDev, &hidattr))
	{
		cerr << "HidGetAddr failed errorno: " << GetLastError() << endl;
		CloseHandle(hHidDev);
		return;
	}

	_tprintf(_T("0x%x, 0x%x, 0x%x \n"), hidattr.VendorID, hidattr.ProductID, hidattr.VersionNumber);

	ULONG ulBufSize = 32;
	wchar_t tcBUf[32] = { 0 };
	if (!HidD_GetProductString(hHidDev, tcBUf, ulBufSize))
	{
		cerr << " HidD_GetProductString failed error no: " << GetLastError() << endl;
		CloseHandle(hHidDev);
		return;
	}

	printf("Product String: %ls \n", tcBUf);

	PHIDP_PREPARSED_DATA PreparseData;
	if (!HidD_GetPreparsedData(hHidDev, &PreparseData))
	{
		cerr << " HidD_GetPreparseData failed error no: " << GetLastError() << endl;
		CloseHandle(hHidDev);
		return;
	}

	HIDP_CAPS hidp;
	PHIDP_CAPS phipcaps = &hidp;
	if (!HidP_GetCaps(PreparseData, phipcaps))
	{
		cerr << " Hidp_GetCaps failed error no: " << GetLastError() << endl;
		CloseHandle(hHidDev);
		return;
	}

	/*ULONG ulBufS = phipcaps->InputReportByteLength;
	char cBuf[60] = { 0 };
	if (!HidD_GetInputReport(hHidDev, cBuf, phipcaps->InputReportByteLength))
	{
		cerr << "GetInputReport failed error no: " << GetLastError() << endl;
		CloseHandle(hHidDev);
		return;
	}*/
}

void EnumerateHIDDevices() {
	// 1. 获取 HID 设备的 GUID
	GUID hidGuid;
	HidD_GetHidGuid(&hidGuid);

	if (hidGuid.Data1)
	{
		//wcout << hidGuid.Data1 << static_cast<void*>(L"-") << hidGuid.Data2 << static_cast<void*>(L"-") << hidGuid.Data3 << static_cast<void*>(L"-") << hidGuid.Data4 << endl;	
		wcout << hidGuid.Data1 <<L"-" << hidGuid.Data2 << L"-" << hidGuid.Data3 << L"-" << hidGuid.Data4 << endl;
	}

	// 2. 枚举设备接口
	HDEVINFO hDevInfo = SetupDiGetClassDevs(
		&hidGuid,          // 使用获取的 HID GUID
		NULL,              // 不指定设备实例 ID
		NULL,              // 默认窗口句柄
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE  // 仅枚举当前存在的设备接口
	);

	if (hDevInfo == INVALID_HANDLE_VALUE) {
		printf("SetupDiGetClassDevs failed with error: %lu\n", GetLastError());
		return;
	}

	// 3. 遍历设备接口
	SP_DEVICE_INTERFACE_DATA devInfoData = { 0 };
	devInfoData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

	FILE* fp = NULL;
	fopen_s(&fp, "devpath.txt", "wt+");
	assert(fp);
	fseek(fp, 0, SEEK_SET);

	for (DWORD i = 0; SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &hidGuid, i, &devInfoData); i++) 
	{
		// 4. 获取设备路径
		DWORD requiredSize = 0;
		SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, NULL, 0, &requiredSize, NULL);

		PSP_INTERFACE_DEVICE_DETAIL_DATA pDevInfoDetailData = (PSP_INTERFACE_DEVICE_DETAIL_DATA)malloc(requiredSize);
		
		if (pDevInfoDetailData)
		{
			pDevInfoDetailData->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		}
		
		if (SetupDiGetDeviceInterfaceDetail(hDevInfo, &devInfoData, pDevInfoDetailData, requiredSize, NULL, NULL)) {
			_tprintf(TEXT("\n Device Path: %s \n"), pDevInfoDetailData->DevicePath);
			fprintf(fp, " %s \n", pDevInfoDetailData->DevicePath);
		}

		assert(pDevInfoDetailData);
		GetHidDeviceInfo(pDevInfoDetailData->DevicePath);

		free(pDevInfoDetailData);
	}

	fclose(fp);
	fp == NULL;

	// 5. 清理
	SetupDiDestroyDeviceInfoList(hDevInfo);
}

//
void _SetOutPutReport(LPCSTR strDevPath)
{
	HANDLE devhandle;
	devhandle = CreateFile(strDevPath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE,
		0, OPEN_EXISTING, 0, 0);
	if (devhandle == INVALID_HANDLE_VALUE)
	{
		cerr << " CreateFile failed error no: " << GetLastError() << endl;
	}

	BYTE OutData[4] = { 0,0,0,0 };
	OutData[0] = 0x01;
	DWORD cbWrittenSize;
	OVERLAPPED overlapped;

	for (int i=0; i<10; i++)
	{
		OutData[1] = i;
		/*if (!HidD_SetOutputReport(devhandle, OutData, sizeof(OutData)))
		{
			cerr << "SetOutputReport failed error no: " << GetLastError() << endl;
			CloseHandle(devhandle);
			return;
		}*/

		if (!WriteFile(devhandle, OutData, sizeof(OutData), &cbWrittenSize, &overlapped));
		{
			cerr << "WriteFile failed error no: " << GetLastError() << endl;
			CloseHandle(devhandle);
			return;
		}
 	}

	CloseHandle(devhandle);
}

//
void _GetInputReport(LPCWSTR strDevPath)
{
	HANDLE devhandle;
	/*devhandle = CreateFile("\\\\?\\hid#vid_056e&pid_0197&mi_02#9&20e4ca02&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}", GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);*/
	devhandle = CreateFileW(strDevPath,  0,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
 	if (devhandle == INVALID_HANDLE_VALUE)
	{
		cerr << "CreateFile failed error no:" << GetLastError() << endl;
		CloseHandle(devhandle);
		return;
	}

	OpenFileMapping;

	ULONG ulBufSize = 128;
	UCHAR cBuf[128];
	/*if (!HidD_GetInputReport(devhandle, cBuf, ulBufSize))
	{
		cerr << "GetInputReport failed error no : " << GetLastError() << endl;
		CloseHandle(devhandle);
		return;
	}*/

	ZeroMemory(cBuf, sizeof(cBuf));
	DWORD dwReadSize;
	OVERLAPPED overalpped = {0};
	overalpped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL); 
	if (overalpped.hEvent == NULL)
	{
		cerr << "CreateEvent failed error no: " << GetLastError() << endl;
		return;
	}

	/*if (!DeviceIoControl(devhandle, IOCTL_HID_GET_DRIVER_CONFIG,
		NULL,
		0,
		cBuf,
		ulBufSize,
		&dwReadSize,
		NULL))
	{
		cerr << "DeviceIoControl failed error code: " << GetLastError() << endl;
	}*/

	if (!ReadFile(devhandle, cBuf, sizeof(cBuf), &dwReadSize, 0))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			BOOL ret = GetOverlappedResult(devhandle, &overalpped, &dwReadSize, TRUE);
			if (!ret)
			{
				cerr << "GetOverlappedResult failed error no: " << GetLastError() << endl;
			}
			else
			{
				cout << "Read %lu bytes " << dwReadSize << endl;
				for (DWORD i = 0; i < dwReadSize; i++)
				{
					cout << " %02X " << cBuf[i] << endl;
				}

			}
		}
		else
		{
			cerr << "ReadFile failed error no: " << GetLastError() << endl;
			CloseHandle(devhandle);
			return;
		}
		CloseHandle(devhandle);
		return;
	}
	return;
}

typedef struct hid_device_info* (HID_API_CALL* hidenum_func) (unsigned short vendor_id, unsigned short product_id);
typedef void (HID_API_CALL* hid_free_func) (struct hid_device_info* devs);

void testhiddll()
{
	HMODULE hdll = LoadLibrary(_T("hidapi.dll"));
	if (hdll)
	{
		hidenum_func func_hidenum = (hidenum_func)GetProcAddress(hdll, "hid_enumerate");
		hid_free_func func_hidfree = (hid_free_func)GetProcAddress(hdll, "hid_free_enumeration");

		hid_device_info* device = func_hidenum(0, 0);
		if (!device)
		{
			cerr << "no device error no: " << GetLastError() << endl;
			FreeLibrary(hdll);
			return;
		}

		while (device)
		{
			wprintf(L"vid: 0x%x, pid: 0x%x, Product: %ls \n", device->vendor_id, device->product_id, device->product_string);

			device = device->next;
		}
		
		func_hidfree(device);

		FreeLibrary(hdll);
	}

	CreateFile;

}

int main() 
{
	EnumerateHIDDevices();
	//_GetInputReport(L"\\\\?\\hid#vid_056e&pid_0197&mi_00&col01#9&14d11885&0&0000#{4d1e55b2-f16f-11cf-88cb-001111000030}"); //ERROR_ACCESS

	//testhiddll();
	return 0;
}

#endif







