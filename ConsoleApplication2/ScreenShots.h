#pragma once
#include "windows.h"
#include <atlstr.h>
#include <GdiPlus.h>
#pragma comment(lib,"GdiPlus.lib")
using namespace Gdiplus;

class ScreenShots
{
public:
	ScreenShots(void);
	~ScreenShots(void);
	CString	strFilePath;//文件路径
	bool CScreenShot(HDC hSrcDC, RECT rect,PTCHAR szFileName);//桌面截屏
	bool GetDesktopRect(void); //获取桌面RECT
	BOOL GetEncoderClsid(WCHAR* pFormat,CLSID* pClsid);//获取文件编码方式
	BOOL BMPToPNG(LPCWSTR StrBMp,LPCWSTR StrPNG);//转换BMP文件成PNG
private:
	GdiplusStartupInput m_gdiplusStartupInput;//GDI初始化
	ULONG_PTR m_pGdiToken;
	CString GetTimeNow(void);//获取当前时间作为文件名
};

