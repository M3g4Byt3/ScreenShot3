// ConsoleApplication2.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "ScreenShots.h"
#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ScreenShots *ss = new ScreenShots;
	while(1){
		ss->GetDesktopRect();
		Sleep(1000);
	}

	return 0;

}
