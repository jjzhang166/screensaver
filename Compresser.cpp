/*
 * Compresser.cpp
 *
 *  Created on: 2014年8月25日
 *      Author: zhangbo
 */

#include "Compresser.h"

#include <windows.h>
#include <stdio.h>
#include <iostream>

#define TYPE_PIXEL_CPY 0
#define TYPE_PIXEL_OLD 1
#define TYPE_PIXEL_SAM 2
#define TYPE_CURSOR    3

Compresser::Compresser(const char* file) : IProcessor(file) {
}

Compresser::~Compresser() {
}

static DWORD same_old(DWORD * pSrc, DWORD * pDes, DWORD max) {
	DWORD len = 0;
	while (*(pSrc + len) == *(pDes + len)) {
		len++;
		if (len == max) {
			break;
		}
	}
	return len;
}

static DWORD same_new(DWORD * pDes, DWORD max) {
	DWORD len = 0;

	while (*(pDes + len) == *pDes) {
		len++;
		if (len == max) {
			break;
		}
	}

	return len;
}

static void writeto(std::fstream& file, DWORD type, DWORD size, DWORD *data,
		DWORD len) {
	file.write((char*) &type, 4);
	file.write((char*) &size, 4);
	if (len > 0) {
		file.write((char*) data, len * sizeof(DWORD));
	}
}

static void Compress01(DWORD * pSrc, DWORD * pDes, std::fstream& file,
		DWORD size) {
	DWORD copyLeng = 0;
	DWORD current = 0;
	DWORD copyFrom = current;
	while (current < size) {
		DWORD toOld = same_old(pSrc + current, pDes + current, size - current); //与上幅画面相同的长度
		DWORD toSam = same_new(pDes + current, size - current); //像素相同的长度
		if (toOld > toSam && toOld > 10) {
			if (copyLeng > 0) {
				writeto(file, TYPE_PIXEL_CPY, copyLeng, pDes + copyFrom,
						copyLeng);
			}
			writeto(file, TYPE_PIXEL_OLD, toOld, NULL, 0);
			current += toOld;
			copyFrom = current;
			copyLeng = 0;
		} else if (toSam > toOld && toSam > 10) {
			if (copyLeng > 0) {
				writeto(file, TYPE_PIXEL_CPY, copyLeng, pDes + copyFrom,
						copyLeng);
			}
			writeto(file, TYPE_PIXEL_SAM, toSam, pDes + current, 1);
			current += toSam;
			copyFrom = current;
			copyLeng = 0;
		} else {
			current++;
			copyLeng++;
		}
	}
	if (copyLeng > 0) {
		writeto(file, TYPE_PIXEL_CPY, copyLeng, pDes + copyFrom, copyLeng);
	}
}

void* Compresser::Open() {
	lpData[0] = new unsigned char[w * h * 4];
	lpData[1] = new unsigned char[w * h * 4];
	memset(lpData[1], 0xFF, w * h * 4);

	fout.open(file, std::ios::binary | std::ios::out);
	return lpData[0];
}

void Compresser::WriteCursor(long x, long y) {
	DWORD t = TYPE_CURSOR;
	fout.write((char*)&t, sizeof(t));
	fout.write((char*)&x, sizeof(x));
	fout.write((char*)&y, sizeof(y));
}

void Compresser::Write(void* data, unsigned int size) {
	fout.write((char*)data, size);
}

void* Compresser::WriteFrame(void* frame) {
	void * prev;
	if (frame == lpData[0])
		prev = lpData[1];
	else
		prev = lpData[0];

	Compress01((DWORD*) prev, (DWORD*) frame, fout,
			w * h);
	return prev;
}

void Compresser::Close(void* frame) {
	delete[] (char*) frame;
	fout.close();
}
