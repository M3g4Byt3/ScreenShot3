// ConsoleApplication2.cpp : �������̨Ӧ�ó������ڵ㡣
//
#include "ScreenShots.h"
#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ScreenShots *ss = new ScreenShots(".\\sn\\");//���캯�������ļ�����·��
	while(1){
		ss->GetDesktopRect();
		Sleep(5000);
	}

	return 0;

}
