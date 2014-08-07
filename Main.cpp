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
		HDC hDeskDC = GetWindowDC(hWnd);//��ȡ���滭������
		LPRECT lpRect = new RECT;
		GetClientRect(hWnd, lpRect);//��þ�������
		HDC hMemDC = CreateCompatibleDC(hDeskDC);					//����һ�����ݵĻ���
		HBITMAP hBmp = CreateCompatibleBitmap(hDeskDC, lpRect->right-lpRect->left,
				lpRect->bottom-lpRect->top);	//��������λͼ
		SelectObject(hMemDC, hBmp);							//ѡ��λͼ����
		BITMAP bitmap;
		GetObject(hBmp, sizeof(BITMAP), &bitmap);
		BitBlt(hMemDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hDeskDC, 0, 0, SRCCOPY);

		//��ȡ���λ��,���仭�ڵ�ǰλ��
		POINT point;
		GetCursorPos(&point);
		HICON hIcon = (HICON) GetCursor();
		DrawIcon(hMemDC, point.x, point.y, hIcon);

		DWORD size = bitmap.bmWidthBytes * bitmap.bmHeight;

		BYTE* lpData = new BYTE[size];

		int panelsize = 0;									//��¼��ɫ���С
		if (bitmap.bmBitsPixel < 16)							//�ж��Ƿ�Ϊ���ɫλͼ
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
			strhdr.dwRate = 10;//ÿ���Ӳ���10֡
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
		Sleep(100);//ÿ0.1���Ӳɼ�һ��
	}
	AVIStreamClose(pComStream);
	AVIStreamClose(ps);
	if(pfile != NULL)
		AVIFileRelease(pfile);
	AVIFileExit();

	return 0;
}

