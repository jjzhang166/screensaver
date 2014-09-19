/*
 * Compresser.cpp
 *
 *  Created on: 2014��8��25��
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

Compresser::Compresser(const char* file) : IWriter(file) {
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

static void writeto(FileCompressStream& file, DWORD type, DWORD size, DWORD *data,
		DWORD len) {
	file.Write((unsigned char*) &type, 4);
	file.Write((unsigned char*) &size, 4);
	if (len > 0) {
		file.Write((unsigned char*) data, len * sizeof(DWORD));
	}
}

static void Compress01(DWORD * pSrc, DWORD * pDes, FileCompressStream& file,
		DWORD size) {
	DWORD copyLeng = 0;
	DWORD current = 0;
	DWORD copyFrom = current;
	while (current < size) {
		DWORD toOld = same_old(pSrc + current, pDes + current, size - current); //���Ϸ�������ͬ�ĳ���
		DWORD toSam = same_new(pDes + current, size - current); //������ͬ�ĳ���
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

Frame* Compresser::Open(unsigned int w, unsigned int h) {
	this->w = w;
	this->h = h;
	frames[0].frame = new unsigned char[w * h * 4];
	frames[1].frame = new unsigned char[w * h * 4];
	memset(frames[1].frame, 0xFF, w * h * 4);

	fout.Open(file, Z_DEFAULT_COMPRESSION);
	return &(frames[0]);
}

void Compresser::Write(void* data, unsigned int size) {
	fout.Write((unsigned char*)data, size);
}

Frame* Compresser::WriteFrame(Frame* f) {
	Frame* prev;
	if (f == &(frames[0]))
		prev = &(frames[1]);
	else
		prev = &(frames[0]);

	DWORD t = TYPE_CURSOR;
	fout.Write((unsigned char*)&t, sizeof(t));
	fout.Write((unsigned char*)&f->x, sizeof(f->x));
	fout.Write((unsigned char*)&f->y, sizeof(f->y));
	fout.Write((unsigned char*)&f->type, sizeof(f->type));

	Compress01((DWORD*) prev->frame, (DWORD*) f->frame, fout,
			w * h);

	return prev;
}

void Compresser::Close() {
	delete frames[0].frame;
	delete frames[1].frame;
	fout.Close();
}
