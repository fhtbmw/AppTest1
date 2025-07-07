#include "pch.h"
#include"BoostTest7.h"
#include"BoostTest6.h"
#include<windows.h>

//int main() {
//	TestAssert();
//	//test7::TestFormat();
//	return 0;
//}


typedef struct _GAME_MODE
{
	BYTE        cbWinFlag;
	BYTE        cbAltTabFlag;
	BYTE        cbAltF4Flag;
	BYTE        cbShiftTabFlag;
	BYTE        cbTidleFlag;
	BYTE        cbChgFlag;
	BYTE        cbNchgFlag;
	BYTE        cbCapsFlag;
	BYTE        cbRomaFlag;
}GAME_MODE, * PGAME_MODE;

void _Com()
{
	_GAME_MODE g1,g2;
	g1.cbCapsFlag = 1;
	g1.cbAltF4Flag = 0;

	g2.cbAltF4Flag = 0;
	g2.cbCapsFlag = 1;
	cout<<memcmp(&g1,&g2,sizeof(g1));
}

int main(){
	_Com();
	//HINSTANCE m=LoadLibrary(L"elecom_api.dll");
	////HINSTANCE m=LoadLibrary(L"dll1.dll");
	//cout<<GetCurrentProcessId()<< endl;
	//	if (m == nullptr)
	//	{

	//		cout << "load error" << GetLastError() << endl;
	//	}
	//	else
	//	{
	//		cout << "load success" << endl;
	//	}
	//	char falg;
	//	cin >> falg;



	return 0;

}