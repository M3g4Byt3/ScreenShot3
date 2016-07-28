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

/*
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
*/


bool ScreenShots::CScreenShot(HDC hSrcDC, RECT rect,PTCHAR szFileName)
{
	int nWidth = rect.right - rect.left;
	int nHeight = rect.bottom - rect.top;

	if(!nWidth || !nHeight)
		return FALSE;

	BITMAP bmp; //GetObject 信息
	WORD cClrBits; //实际用到的颜色数
	//PBITMAPINFO pbmi; // DIB 的维度和颜色信息 defines the dimensions and color information for a DIB.
	HDC hMemDC = CreateCompatibleDC(hSrcDC); //创建兼容DC，必须释放
	HBITMAP hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight); //创建桌面DC位图
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); //选入
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, rect.left, rect.top, SRCCOPY);

	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp))//获取兼容BIT信息
		return FALSE;

	bmp.bmBitsPixel = 16;//16bpp

	// Convert the color format to a count of bits. 
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1) 
		cClrBits = 1; 
	else if (cClrBits <= 4) 
		cClrBits = 4; 
	else if (cClrBits <= 8) 
		cClrBits = 8; 
	else if (cClrBits <= 16) 
		cClrBits = 16; 
	else if (cClrBits <= 24) 
		cClrBits = 24; 
	else 
		cClrBits = 32;

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);//选出

	PVOID lpvpxldata;//图像真实数据指针
	DWORD dwBitmapArraySize;//一个扫描行所占的真实字节数*nHeight
	DWORD nBitsOffset;//图像数据偏移
	DWORD lImageSize;//文件数据大小
	DWORD lFileSize;//整个文件大小

	BITMAPFILEHEADER bmFileHeader;//BMP文件头
	PBITMAPINFO bmInfo;//图像信息指针
	
	HANDLE hbmfile;//文件句柄
	DWORD dwWritten;

	//卧槽，默认32位cClrBits
	//dwBitmapArraySize = ((((nWidth*32) + 31) & ~31) >> 3) * nHeight;
	dwBitmapArraySize = ((((nWidth*cClrBits) + 31) & ~31) >> 3) * nHeight;
	//它用来在指定的堆上分配内存，并且分配后的内存不可移动。
	//如果成功分配内存，返回值为一个指向所分配内存块的首地址的（void*）指针。
	lpvpxldata = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, dwBitmapArraySize);
	ZeroMemory(lpvpxldata, dwBitmapArraySize);//初始化图像大小空间，在此之前必须初始化lpvpxldata指针指向HeapAlloc分配的内存空间
	//ZeroMemory(bmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD) * (1<< cClrBits));//这里需要加入调色板
	//bmInfo = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); //（分配内存）图像信息和调色板
	bmInfo = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));//调色板好像没有计算

	// Locks a local memory object and returns a pointer to the first byte of the object's memory block.
	if(bmInfo != NULL){
		LocalLock(bmInfo);
	}

	//图像信息头
	bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//biSize是否包含了调色板大小？好像没有
	bmInfo->bmiHeader.biWidth = bmp.bmWidth;
	bmInfo->bmiHeader.biHeight = bmp.bmHeight;
	bmInfo->bmiHeader.biPlanes = bmp.bmPlanes;//必须为1
	bmInfo->bmiHeader.biBitCount = bmp.bmBitsPixel;// 每个像素所占的位数（bit）biBitCount = bmBitsPixel*bmPlanes => bmBitsPixel
	if (cClrBits < 24) 
		bmInfo->bmiHeader.biClrUsed = (1<<cClrBits); 
	bmInfo->bmiHeader.biCompression = BI_RGB;

	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	nBitsOffset = sizeof(BITMAPFILEHEADER) + bmInfo->bmiHeader.biSize;//图像数据偏移
	//这里用了bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount（cClrBits）
	lImageSize = ((((bmInfo->bmiHeader.biWidth * bmInfo->bmiHeader.biBitCount) + 31) & ~31)>> 3) * bmInfo->bmiHeader.biHeight;
	
	lFileSize = nBitsOffset + lImageSize;
	bmFileHeader.bfType = 'B' + ('M'<<8);
	bmFileHeader.bfSize = lFileSize;//整个位图文件大小 (BITMAPFILEHEADER + biSize + biClrUsed * sizeof(RGBQUAD)) + biSizeImage in byte
	bmFileHeader.bfOffBits = nBitsOffset;

	GetDIBits(hMemDC, hBitmap, 0, bmInfo->bmiHeader.biHeight, lpvpxldata, bmInfo, DIB_RGB_COLORS);
	//创建文件
	hbmfile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hbmfile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, 0);//写文件头
	WriteFile(hbmfile, bmInfo, sizeof(BITMAPINFO), &dwWritten, 0);//写信息表
	WriteFile(hbmfile, lpvpxldata, lImageSize, &dwWritten, 0);//写位图真实数据
	CloseHandle(hbmfile);
	//释放内存
	LocalUnlock(bmInfo);//解锁bmInfo内存
	LocalFree(bmInfo); //释放LocalAlloc pbmi
	HeapFree(GetProcessHeap(), HEAP_NO_SERIALIZE, lpvpxldata);
	ReleaseDC(hWnd, hSrcDC);
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
	return TRUE;
}




bool ScreenShots::GetDesktopRect(void)
{
	RECT rc;
	HDC hSrcDC;
	hWnd = GetDesktopWindow();
	hSrcDC = GetDC(hWnd);

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
	//BMPToPNG(bmpFilePath,pngFilePath);
	//DeleteFile(bmpFilePath);
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
