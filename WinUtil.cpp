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

#include "Recorder.h"
#include "Compresser.h"
#include "Uncompresser.h"

#define TYPE_PIXEL_CPY 0
#define TYPE_PIXEL_OLD 1
#define TYPE_PIXEL_SAM 2
#define TYPE_CURSOR    3

WinUtil::WinUtil() {

}

WinUtil::~WinUtil() {

}

int WinUtil::SaveScreenToBmps(const char* path) {
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
	BYTE* lpData = new BYTE[size];
	memset(lpData, 0, size);

	BITMAPFILEHEADER bfh;
	memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
	bfh.bfType = ((WORD) ('M' << 8) | 'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(bmpInfo.bmiHeader) + sizeof(bfh);
	bfh.bfSize = bfh.bfOffBits + size;

	for (int i = 0; i < 100; i++) {
		BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hDeskDC, 0, 0,
		SRCCOPY);
		GetDIBits(hMemDC, hBmp, 0, bmpInfo.bmiHeader.biHeight, lpData, &bmpInfo,
		DIB_RGB_COLORS);
		char str[128];
		fstream fout;
		sprintf(str, "%s/%03d.bmp", path, i);
		fout.open(str, ios::binary | ios::out);
		fout.write((char*) &bfh, sizeof(bfh));
		fout.write((char*) &(bmpInfo.bmiHeader), sizeof(bmpInfo.bmiHeader));
		fout.write((char*) lpData, size);
		fout.flush();
		fout.close();
		Sleep(100);
	}

	delete[] lpData;
	DeleteObject(hBmp);
	DeleteObject(hMemDC);

	return 0;
}

int WinUtil::RecordScreenToFile(const char* file, DWORD time) {
	Compresser c(file);
	Recorder r(&c);
	ThreadCreator::StartThread(&r);
	Sleep(time);
	r.Stop();
	Sleep(10*1000);
	return 0;
}

/**
 * 将文件夹下的标准BMP文件，按照顺序压缩为Record文件。
 */
int WinUtil::BmpsToRecordFile(const char* path, const char* file) {
	BMPHEADER bh;
	Frame* frame = NULL;
	Compresser* writer = new Compresser(file);
	char str[256];
	int i = 0;
	while (true) {
		fstream fin;
		sprintf(str, "%s/%03d.bmp", path, i);
		fin.open(str, ios::binary | ios::in);
		if (!fin) {
			break;
		}
		fin.read((char*) &bh, sizeof(bh));
		if (i == 0) {
			frame = writer->Open(bh.info.biWidth, bh.info.biHeight);
			writer->Write(&bh, sizeof(bh));
		}
		fin.read((char*) (frame->frame), bh.info.biSizeImage);

		frame = writer->WriteFrame(frame);

		fin.close();
		i++;
	}

	writer->Close();
	delete writer;
	return 0;
}

int inf(FILE *source, FILE *dest);
/**
 * 将Record文件，解压缩为标准的AVI文件。
 */
int WinUtil::RecordFileToAvi(const char* avi, const char* file, DWORD dwQuality,
		DWORD dwScale, DWORD dwRate) {

	char tmp[1024];
	sprintf(tmp, "%s.tmp", file);

	FILE* filei = fopen(file, "rb");
	FILE* fileo = fopen(tmp, "wb");
	inf(filei, fileo);
	fclose(filei);
	fclose(fileo);

	Uncompresser uc;
	PBMPHEADER ph = uc.Open(tmp);
	Frame* frame = NULL;

	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps;
	PAVISTREAM pComStream;
	AVICOMPRESSOPTIONS pCompressOption;
	AVICOMPRESSOPTIONS FAR * opts[1] = { &pCompressOption };

	AVIFileInit();

	AVIFileOpen(&pfile, avi, OF_WRITE | OF_CREATE, NULL);
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.fccType = streamtypeVIDEO;
	strhdr.fccHandler = 0;
	strhdr.dwScale = dwScale;
	strhdr.dwRate = dwRate;							//每秒钟播放10帧
	strhdr.dwSuggestedBufferSize = ph->info.biSizeImage;
	SetRect(&strhdr.rcFrame, 0, 0, ph->info.biWidth, ph->info.biHeight);
	AVIFileCreateStream(pfile, &ps, &strhdr);

	opts[0]->fccType = streamtypeVIDEO;
	opts[0]->fccHandler = mmioStringToFOURCC("MSVC", 0);
	opts[0]->dwQuality = dwQuality;
	opts[0]->dwBytesPerSecond = 0;
	opts[0]->dwFlags = AVICOMPRESSF_VALID || AVICOMPRESSF_KEYFRAMES;
	opts[0]->lpFormat = 0;
	opts[0]->cbFormat = 0;
	opts[0]->dwInterleaveEvery = 0;

	AVIMakeCompressedStream(&pComStream, ps, &pCompressOption, NULL);
	AVIStreamSetFormat(pComStream, 0, &(ph->info), sizeof(BITMAPINFOHEADER));

	int i = 0;
	while ((frame = uc.ReadFrame()) != NULL) {
		AVIStreamWrite(pComStream, i++, 1, (LPBYTE) frame->frame,
				ph->info.biSizeImage, AVIIF_KEYFRAME, NULL, NULL);
	}

	uc.Close();

	AVIStreamClose(pComStream);
	AVIStreamClose(ps);
	if (pfile != NULL)
		AVIFileRelease(pfile);
	AVIFileExit();

	Util::DeleteFile1(tmp);

	return 0;
}

int WinUtil::RecordFileToBmps(const char* path, const char* file) {
	Uncompresser uc;
	PBMPHEADER ph = uc.Open(file);
	DWORD size = ph->info.biSizeImage;
	Frame* frame = NULL;
	int i = 0;
	while ((frame = uc.ReadFrame()) != NULL) {
		char str[128];
		std::fstream fout;
		sprintf(str, "%s/%03d.bmp", path, i++);
		fout.open(str, ios::binary | ios::out);
		fout.write((char*) ph, sizeof(BMPHEADER));
		fout.write((char*) frame->frame, size);
		fout.close();
	}
	uc.Close();

	return 0;
}

int WinUtil::SaveScreenToAvi(const char* file, DWORD count) {

	AVISTREAMINFO strhdr;
	PAVIFILE pfile;
	PAVISTREAM ps;
	PAVISTREAM pComStream;
	int nFrames = 0;
	AVICOMPRESSOPTIONS pCompressOption;
	AVICOMPRESSOPTIONS FAR * opts[1] = { &pCompressOption };

	AVIFileInit();

	for (unsigned int i = 0; i < count; i++) {
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
			AVIFileOpen(&pfile, file, OF_WRITE | OF_CREATE, NULL);
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
