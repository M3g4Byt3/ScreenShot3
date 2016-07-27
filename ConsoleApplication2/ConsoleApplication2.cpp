// ConsoleApplication2.cpp : 定义控制台应用程序的入口点。
//
#include "ScreenShots.h"
#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ScreenShots *ss = new ScreenShots(".\\sn\\");//构造函数传入文件保存路径
	while(1){
		ss->GetDesktopRect();
		Sleep(5000);
	}

	return 0;

}
