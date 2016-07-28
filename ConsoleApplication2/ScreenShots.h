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
	ScreenShots::ScreenShots(CString saveFilePath);//���ع��캯���������ļ�·��
	~ScreenShots(void);
	CString strFilePath;//�ļ�·��
	HWND hWnd;//������
	bool CScreenShot(HDC hSrcDC, RECT rect,PTCHAR szFileName);//�������
	bool GetDesktopRect(void); //��ȡ����RECT
	BOOL GetEncoderClsid(WCHAR* pFormat,CLSID* pClsid);//��ȡ�ļ����뷽ʽ
	BOOL BMPToPNG(LPCWSTR StrBMp,LPCWSTR StrPNG);//ת��BMP�ļ���PNG
private:
	GdiplusStartupInput m_gdiplusStartupInput;//GDI��ʼ��
	ULONG_PTR m_pGdiToken;
	CString GetTimeNow(void);//��ȡ��ǰʱ����Ϊ�ļ���
};

