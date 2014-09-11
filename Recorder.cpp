/*
 * Recorder.cpp
 *
 *  Created on: 2014年9月9日
 *      Author: zhangbo
 */

#include "Recorder.h"

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

Recorder::Recorder(IProcessor* processor) : running(false) {
	this->processor = processor;
}

Recorder::~Recorder() {
}

void Recorder::Run() {
	this->running = true;
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

	void* frame = processor->Open(bitmap.bmWidth, bitmap.bmHeight);

	BITMAPFILEHEADER bfh;
	memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
	bfh.bfType = ((WORD) ('M' << 8) | 'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(bmpInfo.bmiHeader) + sizeof(bfh);
	bfh.bfSize = bfh.bfOffBits + size;

	processor->Write(&bfh, sizeof(bfh));
	processor->Write(&(bmpInfo.bmiHeader), sizeof(bmpInfo.bmiHeader));

	//获取鼠标位置
	POINT point;
	POINT lastp;
	GetCursorPos(&point);
	processor->WriteCursor(point.x, point.y);
	lastp = point;

	while (running) {
		BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hDeskDC, 0, 0,
		SRCCOPY);

		GetCursorPos(&point);
		if (point.x != lastp.x || point.y  != lastp.y) {
			processor->WriteCursor(point.x, point.y);
			lastp = point;
		}

		GetDIBits(hMemDC, hBmp, 0, bmpInfo.bmiHeader.biHeight, frame,
				&bmpInfo,
				DIB_RGB_COLORS);
		frame = processor->WriteFrame(frame);
		Sleep(100);
	}

	processor->Close(frame);
	DeleteObject(hBmp);
	DeleteObject(hMemDC);

	return;
}

void Recorder::Stop() {
	this->running = false;
}
