#include "ScreenShots.h"


ScreenShots::ScreenShots(void)
{
}

ScreenShots::ScreenShots(CString saveFilePath)
{
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);
	CreateDirectory((LPCTSTR)saveFilePath,NULL);//����sn�ļ���
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

	HDC hMemDC = CreateCompatibleDC(hSrcDC); //��������DC�������ͷ�
	HBITMAP hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight); //��������DCλͼ
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); //ѡ��
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, nx, ny, SRCCOPY);
	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);//ѡ��

	PVOID lpvpxldata;//����ָ��
	DWORD dwBitmapArraySize;//һ��ɨ������ռ����ʵ�ֽ���*nHeight
	DWORD nBitsOffset;//ͼ������ƫ��
	DWORD lImageSize;//�ļ����ݴ�С
	DWORD lFileSize;//�����ļ���С

	BITMAPFILEHEADER bmFileHeader;//BMP�ļ�ͷ
	BITMAPINFO bmInfo;//ͼ����Ϣ
	HANDLE hbmfile;//�ļ����
	DWORD dwWritten;

	//�Բۣ�Ĭ��32λ
	dwBitmapArraySize = ((((nWidth*32) + 31) & ~31) >> 3) * nHeight;
	lpvpxldata = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, dwBitmapArraySize);
	ZeroMemory(lpvpxldata, dwBitmapArraySize);
	ZeroMemory(&bmInfo, sizeof(BITMAPINFO));

	//ͼ����Ϣͷ
	bmInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmInfo.bmiHeader.biWidth = nWidth;
	bmInfo.bmiHeader.biHeight = nHeight;
	bmInfo.bmiHeader.biPlanes = 1;
	bmInfo.bmiHeader.biBitCount = 32;// ÿ��������ռ��λ����bit��
	bmInfo.bmiHeader.biCompression = BI_RGB;

	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	nBitsOffset = sizeof(BITMAPFILEHEADER) + bmInfo.bmiHeader.biSize;//ͼ������ƫ��
	lImageSize = ((((bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount) + 31) & ~31)>> 3) * bmInfo.bmiHeader.biHeight;
	lFileSize = nBitsOffset + lImageSize;
	bmFileHeader.bfType = 'B' + ('M'<<8);
	bmFileHeader.bfSize = lFileSize;//����λͼ�ļ���С
	bmFileHeader.bfOffBits = nBitsOffset;

	GetDIBits(hMemDC, hBitmap, 0, bmInfo.bmiHeader.biHeight, lpvpxldata, &bmInfo, DIB_RGB_COLORS);
	//�����ļ�
	hbmfile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hbmfile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, 0);
	WriteFile(hbmfile, &bmInfo, sizeof(BITMAPINFO), &dwWritten, 0);
	WriteFile(hbmfile, lpvpxldata, lImageSize, &dwWritten, 0);
	CloseHandle(hbmfile);
	//�ͷ��ڴ�
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

	BITMAP bmp; //GetObject ��Ϣ
	WORD cClrBits; //ʵ���õ�����ɫ��
	//PBITMAPINFO pbmi; // DIB ��ά�Ⱥ���ɫ��Ϣ defines the dimensions and color information for a DIB.
	HDC hMemDC = CreateCompatibleDC(hSrcDC); //��������DC�������ͷ�
	HBITMAP hBitmap = CreateCompatibleBitmap(hSrcDC, nWidth, nHeight); //��������DCλͼ
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap); //ѡ��
	BitBlt(hMemDC, 0, 0, nWidth, nHeight, hSrcDC, rect.left, rect.top, SRCCOPY);

	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp))//��ȡ����BIT��Ϣ
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

	hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);//ѡ��

	PVOID lpvpxldata;//ͼ����ʵ����ָ��
	DWORD dwBitmapArraySize;//һ��ɨ������ռ����ʵ�ֽ���*nHeight
	DWORD nBitsOffset;//ͼ������ƫ��
	DWORD lImageSize;//�ļ����ݴ�С
	DWORD lFileSize;//�����ļ���С

	BITMAPFILEHEADER bmFileHeader;//BMP�ļ�ͷ
	PBITMAPINFO bmInfo;//ͼ����Ϣָ��
	
	HANDLE hbmfile;//�ļ����
	DWORD dwWritten;

	//�Բۣ�Ĭ��32λcClrBits
	//dwBitmapArraySize = ((((nWidth*32) + 31) & ~31) >> 3) * nHeight;
	dwBitmapArraySize = ((((nWidth*cClrBits) + 31) & ~31) >> 3) * nHeight;
	//��������ָ���Ķ��Ϸ����ڴ棬���ҷ������ڴ治���ƶ���
	//����ɹ������ڴ棬����ֵΪһ��ָ���������ڴ����׵�ַ�ģ�void*��ָ�롣
	lpvpxldata = HeapAlloc(GetProcessHeap(), HEAP_NO_SERIALIZE, dwBitmapArraySize);
	ZeroMemory(lpvpxldata, dwBitmapArraySize);//��ʼ��ͼ���С�ռ䣬�ڴ�֮ǰ�����ʼ��lpvpxldataָ��ָ��HeapAlloc������ڴ�ռ�
	//ZeroMemory(bmInfo, sizeof(BITMAPINFO) + sizeof(RGBQUAD) * (1<< cClrBits));//������Ҫ�����ɫ��
	//bmInfo = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits)); //�������ڴ棩ͼ����Ϣ�͵�ɫ��
	bmInfo = (PBITMAPINFO) LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));//��ɫ�����û�м���

	// Locks a local memory object and returns a pointer to the first byte of the object's memory block.
	if(bmInfo != NULL){
		LocalLock(bmInfo);
	}

	//ͼ����Ϣͷ
	bmInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);//biSize�Ƿ�����˵�ɫ���С������û��
	bmInfo->bmiHeader.biWidth = bmp.bmWidth;
	bmInfo->bmiHeader.biHeight = bmp.bmHeight;
	bmInfo->bmiHeader.biPlanes = bmp.bmPlanes;//����Ϊ1
	bmInfo->bmiHeader.biBitCount = bmp.bmBitsPixel;// ÿ��������ռ��λ����bit��biBitCount = bmBitsPixel*bmPlanes => bmBitsPixel
	if (cClrBits < 24) 
		bmInfo->bmiHeader.biClrUsed = (1<<cClrBits); 
	bmInfo->bmiHeader.biCompression = BI_RGB;

	ZeroMemory(&bmFileHeader, sizeof(BITMAPFILEHEADER));
	nBitsOffset = sizeof(BITMAPFILEHEADER) + bmInfo->bmiHeader.biSize;//ͼ������ƫ��
	//��������bmInfo.bmiHeader.biWidth * bmInfo.bmiHeader.biBitCount��cClrBits��
	lImageSize = ((((bmInfo->bmiHeader.biWidth * bmInfo->bmiHeader.biBitCount) + 31) & ~31)>> 3) * bmInfo->bmiHeader.biHeight;
	
	lFileSize = nBitsOffset + lImageSize;
	bmFileHeader.bfType = 'B' + ('M'<<8);
	bmFileHeader.bfSize = lFileSize;//����λͼ�ļ���С (BITMAPFILEHEADER + biSize + biClrUsed * sizeof(RGBQUAD)) + biSizeImage in byte
	bmFileHeader.bfOffBits = nBitsOffset;

	GetDIBits(hMemDC, hBitmap, 0, bmInfo->bmiHeader.biHeight, lpvpxldata, bmInfo, DIB_RGB_COLORS);
	//�����ļ�
	hbmfile = CreateFile(szFileName, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	WriteFile(hbmfile, &bmFileHeader, sizeof(BITMAPFILEHEADER), &dwWritten, 0);//д�ļ�ͷ
	WriteFile(hbmfile, bmInfo, sizeof(BITMAPINFO), &dwWritten, 0);//д��Ϣ��
	WriteFile(hbmfile, lpvpxldata, lImageSize, &dwWritten, 0);//дλͼ��ʵ����
	CloseHandle(hbmfile);
	//�ͷ��ڴ�
	LocalUnlock(bmInfo);//����bmInfo�ڴ�
	LocalFree(bmInfo); //�ͷ�LocalAlloc pbmi
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

//��ȡͼƬ�ļ��ı��뷽ʽ
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
