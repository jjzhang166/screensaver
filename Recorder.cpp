/*
 * Recorder.cpp
 *
 *  Created on: 2014Äê9ÔÂ9ÈÕ
 *      Author: zhangbo
 */

#include "Recorder.h"

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <fstream>

Recorder::Recorder(IWriter* writer) : running(false) {
	this->writer = writer;
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

	Frame* frame = writer->Open(bitmap.bmWidth, bitmap.bmHeight);

	BITMAPFILEHEADER bfh;
	memset(&bfh, 0, sizeof(BITMAPFILEHEADER));
	bfh.bfType = ((WORD) ('M' << 8) | 'B');
	bfh.bfReserved1 = 0;
	bfh.bfReserved2 = 0;
	bfh.bfOffBits = sizeof(bmpInfo.bmiHeader) + sizeof(bfh);
	bfh.bfSize = bfh.bfOffBits + size;

	writer->Write(&bfh, sizeof(bfh));
	writer->Write(&(bmpInfo.bmiHeader), sizeof(bmpInfo.bmiHeader));

	while (running) {
		BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hDeskDC, 0, 0,
		SRCCOPY);

		POINT point;
		GetCursorPos(&point);
		frame->x = point.x;
		frame->y = point.y;

		GetDIBits(hMemDC, hBmp, 0, bmpInfo.bmiHeader.biHeight, frame->frame,
				&bmpInfo,
				DIB_RGB_COLORS);
		frame = writer->WriteFrame(frame);
		Sleep(100);
	}

	writer->Close();
	DeleteObject(hBmp);
	DeleteObject(hMemDC);

	return;
}

void Recorder::Stop() {
	this->running = false;
}
