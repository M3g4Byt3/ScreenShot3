#include "ScreenShots.h"


ScreenShots::ScreenShots(void)
{
}

ScreenShots::ScreenShots(CString saveFilePath)
{
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	CreateDirectory((LPCTSTR)saveFilePath,NULL);//创建sn文件夹
	strFilePath = saveFilePath;
}


ScreenShots::~ScreenShots(void)
{
}


bool ScreenShots::CScreenShot(HDC hSrcDC, RECT rect,PTCHAR szFileName)
{
	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom - rect.top;  ;
	int nx = rect.left;			// x of left & top POINT
	int ny = rect.top;			// y of left & top POINT

	if(!nWidth || !nHeight)
		return FALSE;

	HDC hMemDC = CreateCompatibleDC(hSrcDC); //创建兼容DC，必须释放
	HBITMAP hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight); //创建桌面DC位图
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); //选入
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, nx, ny, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);//选出

	PVOID lpvpxldata;//数据指针
	DWORD dwBitmapArraySize;//一个扫描行所占的真实字节数*nHeight
	DWORD nBitsOffset;//图像数据偏移
	DWORD lImageSize;//文件数据大小
	DWORD lFileSize;//整个文件大小

	BITMAPFILEHEADER bmFileHeader;//BMP文件头
	BITMAPINFO bmInfo;//图像信息
	HANDLE hbmfile;//文件句柄
	DWORD dwWritten;

	//卧槽，默认32位
	dwBitmapArraySize = ((((nWidth*32) + 31) & ~31) >> 3) * nHeight;
	lpvpxldata = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, dwBitmapArraySize);
	ZeroMemory(lpvpxldata, dwBitmapArraySize);
	ZeroMemory(&bmInfo, sizeof(BITMAPINFO));

	//图像信息头
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = nWidth;
	bmInfo.bmiHeader.biHeight = nHeight;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;// 每个像素所占的位数（bit）
	bmInfo.bmiHeader.biCompression = BI_RGB;

	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	nBitsOffset = sizeof(BITMAPFILEHEADER) + bmInfo.bmiHeader.biSize;//图像数据偏移
	lImageSize = ((((bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount) + 31) & ~31)>> 3) * bmInfo.bmiHeader.biHeight;
	lFileSize = nBitsOffset + lImageSize;
	bmFileHeader.bfType = 'B' + ('M'<<8);
	bmFileHeader.bfSize = lFileSize;//整个位图文件大小
	bmFileHeader.bfOffBits = nBitsOffset;

	GetDIBits(hMemDC, hBitmap, 0, bmInfo.bmiHeader.biHeight, lpvpxldata, &bmInfo, DIB_RGB_COLORS);
	//创建文件
	hbmfile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hbmfile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, 0);
	WriteFile(hbmfile, &bmInfo, sizeof(BITMAPINFO), &dwWritten, 0);
	WriteFile(hbmfile, lpvpxldata, lImageSize, &dwWritten, 0);
	CloseHandle(hbmfile);
	//释放内存
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpvpxldata);
	ReleaseDC(GetDesktopWindow(), hSrcDC);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	return TRUE;
}


bool ScreenShots::GetDesktopRect(void)
{
	RECT rc;
	HDC hSrcDC;
	//TCHAR bmpFilePath[20] = TEXT(".\\123.bmp"); //字符集真尼玛操心
	HWND hwnd = GetDesktopWindow();
	hSrcDC = GetDC(hwnd);

	rc.left = 0; 
	rc.top = 0; 
	rc.right = GetSystemMetrics (SM_CXSCREEN); //WTF
	rc.bottom = GetSystemMetrics (SM_CYSCREEN);

	/*

	SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);  
	GetClientRect(hwnd,&rc);

	printf("t:%d,",rc.top);
	printf("l:%d --- ",rc.left);
	printf("r:%d\n",rc.right);
	printf("|\n");
	printf("b:%d\n",rc.bottom);
	
	*/

	CString pngFilePath,bmpFilePath,tmpFilePath;
	//strFilePath = ".\\"; default path
	tmpFilePath = strFilePath + GetTimeNow();
	pngFilePath = tmpFilePath +  _T(".PNG");
	bmpFilePath = tmpFilePath +  _T(".BMP");

	TCHAR *pBmp = (TCHAR *)bmpFilePath.GetBuffer();
	bmpFilePath.ReleaseBuffer();

	if(!CScreenShot(hSrcDC,rc,pBmp))
		return FALSE;
	BMPToPNG(bmpFilePath,pngFilePath);
	DeleteFile(bmpFilePath);
	return TRUE;
}



BOOL ScreenShots::BMPToPNG(LPCWSTR StrBMp,LPCWSTR StrPNG)
{
	CLSID encoderClsid;
	Status stat;
	Image* image = NULL;
	image = Bitmap::FromFile(StrBMp,TRUE);
	if (!GetEncoderClsid(L"image/png",&encoderClsid))
	{
		return FALSE;
	}
	stat = image->Save(StrPNG,&encoderClsid,NULL);
	if (stat != Ok)
	{
		return FALSE;
	}
	delete image;
	return TRUE;
}

//获取图片文件的编码方式
BOOL ScreenShots::GetEncoderClsid(WCHAR* pFormat,CLSID* pClsid)
{
	UINT num = 0,size = 0;
	ImageCodecInfo* pImageCodecInfo = NULL;
	GetImageEncodersSize(&num,&size);
	if (size == 0)
	{
		return FALSE;
	}
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
	{
		return FALSE;
	}
	GetImageEncoders(num,size,pImageCodecInfo);
	BOOL bfound = FALSE;
	for (UINT i = 0;!bfound && i < num;  i++)
	{
		if (_wcsicmp(pImageCodecInfo[i].MimeType,pFormat) == 0)
		{
			*pClsid = pImageCodecInfo[i].Clsid;
			bfound = TRUE;
		}
	}
	free(pImageCodecInfo);
	return bfound;
}


CString ScreenShots::GetTimeNow(void)
{
	CString m_date = "";
	SYSTEMTIME time;
	GetLocalTime(&time);
	m_date.Format(_T("%2d.%2d.%2d.%2d.%2d"),time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond);
	return m_date.GetString();
}
