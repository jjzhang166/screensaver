/*
 * update.cpp
 *
 *  Created on: 2012-11-1
 *      Author: zhangbo
 */

#include <windows.h>
#include <cpputils/ThreadCreator.h>
#include <cpputils/Util.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windowsx.h>
#include <cpputils/Properties.h>
#include <vfw.h>

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps;
	PAVISTREAM pComStream;
	int nFrames = 0;
	HRESULT hr;
	AVICOMPRESSOPTIONS pCompressOption;
	AVICOMPRESSOPTIONS FAR * opts[1] = {&pCompressOption};

	AVIFileInit();

	for (int i=0; i<100; i++) {
		HWND hWnd = GetDesktopWindow();
		HDC hDeskDC = GetWindowDC(hWnd);//获取桌面画布对象
		LPRECT lpRect = new RECT;
		GetClientRect(hWnd, lpRect);//获得矩形区域
		HDC hMemDC = CreateCompatibleDC(hDeskDC);					//创建一个兼容的画布
		HBITMAP hBmp = CreateCompatibleBitmap(hDeskDC, lpRect->right-lpRect->left,
				lpRect->bottom-lpRect->top);	//创建兼容位图
		SelectObject(hMemDC, hBmp);							//选中位图对象
		BITMAP bitmap;
		GetObject(hBmp, sizeof(BITMAP), &bitmap);
		BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hDeskDC, 0, 0, SRCCOPY);

		//获取鼠标位置,将其画在当前位置
		POINT point;
		GetCursorPos(&point);
		HICON hIcon = (HICON) GetCursor();
		DrawIcon(hMemDC, point.x, point.y, hIcon);

		DWORD size = bitmap.bmWidthBytes * bitmap.bmHeight;

		BYTE* lpData = new BYTE[size];

		int panelsize = 0;									//记录调色板大小
		if (bitmap.bmBitsPixel < 16)							//判断是否为真彩色位图
			panelsize = pow(2, bitmap.bmBitsPixel * sizeof(RGBQUAD));

		BITMAPINFOHEADER *pBInfo = new BITMAPINFOHEADER;
		pBInfo->biBitCount = bitmap.bmBitsPixel;
		pBInfo->biClrImportant = 0;
		pBInfo->biCompression = 0;
		pBInfo->biHeight = bitmap.bmHeight;
		pBInfo->biPlanes = bitmap.bmPlanes;
		pBInfo->biSize = sizeof(BITMAPINFOHEADER);
		pBInfo->biSizeImage = bitmap.bmWidthBytes * bitmap.bmHeight;
		pBInfo->biWidth = bitmap.bmWidth;
		pBInfo->biXPelsPerMeter = 0;
		pBInfo->biYPelsPerMeter = 0;
		BITMAPINFO bInfo;
		bInfo.bmiHeader = *pBInfo;
		GetDIBits(hMemDC, hBmp, 0, pBInfo->biHeight, lpData, &bInfo,
				DIB_RGB_COLORS);

		if (nFrames == 0) {
			AVIFileOpen(&pfile, "avi.avi", OF_WRITE | OF_CREATE, NULL);
			memset(&strhdr, 0, sizeof(strhdr));
			strhdr.fccType = streamtypeVIDEO;
			strhdr.fccHandler = 0;
			strhdr.dwScale = 1;
			strhdr.dwRate = 10;//每秒钟播放10帧
			strhdr.dwSuggestedBufferSize = pBInfo->biSizeImage;
			SetRect(&strhdr.rcFrame, 0, 0, pBInfo->biWidth, pBInfo->biHeight);
			hr = AVIFileCreateStream(pfile, &ps, &strhdr);

			opts[0]->fccType = streamtypeVIDEO;
			opts[0]->fccHandler = mmioStringToFOURCC("MSVC", 0);
			opts[0]->dwQuality = 7500;
			opts[0]->dwBytesPerSecond = 0;
			opts[0]->dwFlags = AVICOMPRESSF_VALID || AVICOMPRESSF_KEYFRAMES;
			opts[0]->lpFormat = 0;
			opts[0]->cbFormat = 0;
			opts[0]->dwInterleaveEvery = 0;

			AVIMakeCompressedStream(&pComStream, ps, &pCompressOption, NULL);
			AVIStreamSetFormat(pComStream, 0, pBInfo, sizeof(BITMAPINFOHEADER));
		}

		hr = AVIStreamWrite(pComStream, nFrames, 1, (LPBYTE) lpData,
				pBInfo->biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
		nFrames++;

		delete[] lpData;
		delete pBInfo;
		Sleep(100);//每0.1秒钟采集一次
	}
	AVIStreamClose(pComStream);
	AVIStreamClose(ps);
	if(pfile != NULL)
		AVIFileRelease(pfile);
	AVIFileExit();

	return 0;
}

