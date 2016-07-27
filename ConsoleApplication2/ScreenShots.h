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
	bool CScreenShot(HDC hSrcDC, RECT rect,PTCHAR szFileName);
	bool GetDesktopRect(void);
	BOOL GetEncoderClsid(WCHAR* pFormat,CLSID* pClsid);
	BOOL BMptoPNG(LPCWSTR StrBMp,LPCWSTR StrPNG);
private:
	GdiplusStartupInput m_gdiplusStartupInput;
	ULONG_PTR m_pGdiToken;
public:
	CString GetTimeNow(void);
};

