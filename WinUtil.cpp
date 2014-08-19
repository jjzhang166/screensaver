/*
 * WinUtil.cpp
 *
 *  Created on: 2014年8月18日
 *      Author: zhangbo
 */

#include "WinUtil.h"
#include <cpputils/ThreadCreator.h>
#include <cpputils/Util.h>
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <windowsx.h>
#include <cpputils/Properties.h>
#include <vfw.h>
#include <fstream>

WinUtil::WinUtil() {
	// TODO Auto-generated constructor stub

}

WinUtil::~WinUtil() {
	// TODO Auto-generated destructor stub
}

void WinUtil::Compress(BYTE * pSrc, BYTE * pDes, std::fstream& file,
		DWORD size) {
	BYTE *pOut = new BYTE[size];
	BYTE *pUn = new BYTE[size];
	for (DWORD i = 0; i < size; i++) {
		if (pDes[i] == pSrc[i]) {
			pOut[i] = 0;
			pUn[i] = 1;
		} else {
			pOut[i] = pDes[i];
			pUn[i] = 0;
		}
	}
	file.write((char*) pOut, size);
	file.write((char*) pUn, size);
	delete[] pOut;
	delete[] pUn;
}

DWORD WinUtil::Compress(BYTE * pSrc, BYTE * pDes, BYTE * pOut, BYTE* pUn,
		DWORD size) {
	for (DWORD i = 0; i < size; i++) {
		if (pDes[i] == pSrc[i]) {
			pOut[i] = 0;
			pUn[i] = 1;
		} else {
			pOut[i] = pDes[i];
			pUn[i] = 0;
		}
	}
	return size;
}

DWORD WinUtil::UnCompress(BYTE * pSrc, BYTE * pIn, BYTE * pDes, BYTE* pUn,
		DWORD size) {
	for (DWORD i = 0; i < size; i++) {
		if (pUn[i] == 1) {
			pDes[i] = pSrc[i];
		} else {
			pDes[i] = pIn[i];
		}
	}
	return size;
}

int WinUtil::PlayScreen(const char* file) {
	char str[128];
	std::fstream fin;
	sprintf(str, file);
	fin.open(str, ios::binary | ios::in);
	BITMAPFILEHEADER bfh;
	BITMAPINFOHEADER bmiHeader;
	fin.read((char*) &bfh, sizeof(bfh));
	fin.read((char*) &bmiHeader, sizeof(bmiHeader));

	DWORD size = bmiHeader.biSizeImage;
	BYTE* lpData[2];
	lpData[0] = new BYTE[size];
	lpData[1] = new BYTE[size];
	BYTE *pIn = new BYTE[size];
	BYTE *pUn = new BYTE[size];
	memset(lpData[1], 0, size);
	for (int i = 0; i < 100; i++) {
		fin.read((char*) pIn, size);
		fin.read((char*) pUn, size);
		UnCompress(lpData[(i + 1) % 2], pIn, lpData[i % 2], pUn, size);

		char str[128];
		std::fstream fout;
		sprintf(str, "./out/o%03d.bmp", i);
		fout.open(str, ios::binary | ios::out);
		fout.write((char*) &bfh, sizeof(bfh));
		fout.write((char*) &bmiHeader, sizeof(bmiHeader));
		fout.write((char*) lpData[i % 2], size);
		fout.close();
	}
	fin.close();

	delete[] lpData[0];
	delete[] lpData[1];

	return 0;
}

int WinUtil::RecordScreen(const char* file) {
	HWND hWnd = GetDesktopWindow();
	HDC hDeskDC = GetWindowDC(hWnd);
	LPRECT lpRect = new RECT;
	GetClientRect(hWnd, lpRect);
	HDC hMemDC = CreateCompatibleDC(hDeskDC);
	HBITMAP hBmp = CreateCompatibleBitmap(hDeskDC, lpRect->right - lpRect->left,
			lpRect->bottom - lpRect->top);
	SelectObject(hMemDC, hBmp);
	BITMAP bitmap;
	GetObject(hBmp, sizeof(BITMAP), &bitmap);

	BITMAPINFO bmpInfo;
	bmpInfo.bmiHeader.biBitCount = bitmap.bmBitsPixel;
	bmpInfo.bmiHeader.biClrImportant = 0;
	bmpInfo.bmiHeader.biCompression = 0;
	bmpInfo.bmiHeader.biHeight = bitmap.bmHeight;
	bmpInfo.bmiHeader.biPlanes = bitmap.bmPlanes;
	bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmpInfo.bmiHeader.biSizeImage = bitmap.bmWidthBytes * bitmap.bmHeight;
	bmpInfo.bmiHeader.biWidth = bitmap.bmWidth;
	bmpInfo.bmiHeader.biXPelsPerMeter = 0;
	bmpInfo.bmiHeader.biYPelsPerMeter = 0;

	DWORD size = bitmap.bmWidthBytes * bitmap.bmHeight;
	BYTE* lpData[2];
	lpData[0] = new BYTE[size];
	lpData[1] = new BYTE[size];
	memset(lpData[1], 0, size);

	BITMAPFILEHEADER bfh;
	memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
	bfh.bfType = ((WORD) ('M' << 8) | 'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(bmpInfo.bmiHeader) + sizeof(bfh);
	bfh.bfSize = bfh.bfOffBits + size;

	char str[128];
	fstream fout;
	sprintf(str, file);
	fout.open(str, ios::binary | ios::out);
	fout.write((char*) &bfh, sizeof(bfh));
	for (int i = 0; i < 100; i++) {
		BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hDeskDC, 0, 0,
		SRCCOPY);
		GetDIBits(hMemDC, hBmp, 0, bmpInfo.bmiHeader.biHeight, lpData[i % 2],
				&bmpInfo,
				DIB_RGB_COLORS);
		if (i == 0) {
			fout.write((char*) &(bmpInfo.bmiHeader), sizeof(bmpInfo.bmiHeader));
		}
		Compress(lpData[(i + 1) % 2], lpData[i % 2], fout, size);
		Sleep(100);
	}
	fout.flush();
	fout.close();

	delete[] lpData[0];
	delete[] lpData[1];
	DeleteObject(hBmp);
	DeleteObject(hMemDC);

	return 0;
}

int WinUtil::Create() {

	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps;
	PAVISTREAM pComStream;
	int nFrames = 0;
	AVICOMPRESSOPTIONS pCompressOption;
	AVICOMPRESSOPTIONS FAR * opts[1] = { &pCompressOption };

	AVIFileInit();

	for (int i = 0; i < 100; i++) {
		HWND hWnd = GetDesktopWindow();
		HDC hDeskDC = GetWindowDC(hWnd); //获取桌面画布对象
		LPRECT lpRect = new RECT;
		GetClientRect(hWnd, lpRect); //获得矩形区域
		HDC hMemDC = CreateCompatibleDC(hDeskDC);					//创建一个兼容的画布
		HBITMAP hBmp = CreateCompatibleBitmap(hDeskDC,
				lpRect->right - lpRect->left, lpRect->bottom - lpRect->top);//创建兼容位图
		SelectObject(hMemDC, hBmp);							//选中位图对象
		BITMAP bitmap;
		GetObject(hBmp, sizeof(BITMAP), &bitmap);
		BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hDeskDC, 0, 0,
		SRCCOPY);

		//获取鼠标位置,将其画在当前位置
		POINT point;
		GetCursorPos(&point);
		HICON hIcon = (HICON) GetCursor();
		DrawIcon(hMemDC, point.x, point.y, hIcon);

		DWORD size = bitmap.bmWidthBytes * bitmap.bmHeight;

		BYTE* lpData = new BYTE[size];

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
			strhdr.dwRate = 10;							//每秒钟播放10帧
			strhdr.dwSuggestedBufferSize = pBInfo->biSizeImage;
			SetRect(&strhdr.rcFrame, 0, 0, pBInfo->biWidth, pBInfo->biHeight);
			AVIFileCreateStream(pfile, &ps, &strhdr);

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

		AVIStreamWrite(pComStream, nFrames, 1, (LPBYTE) lpData,
				pBInfo->biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
		nFrames++;

		delete[] lpData;
		delete pBInfo;
		Sleep(100);							//每0.1秒钟采集一次
	}
	AVIStreamClose(pComStream);
	AVIStreamClose(ps);
	if (pfile != NULL)
		AVIFileRelease(pfile);
	AVIFileExit();

	return 0;

}
