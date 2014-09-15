/*
 * Uncompresser.cpp
 *
 *  Created on: 2014年9月11日
 *      Author: zhangbo
 */

#include "Uncompresser.h"

#include <windows.h>
#include <stdio.h>

#define TYPE_PIXEL_CPY 0
#define TYPE_PIXEL_OLD 1
#define TYPE_PIXEL_SAM 2
#define TYPE_CURSOR    3

Uncompresser::Uncompresser() :
		index(0) {
}

Uncompresser::~Uncompresser() {
}

PBMPHEADER Uncompresser::Open(const char* file) {
	this->file.open(file, std::ios::binary | std::ios::in);
	this->file.read((char*) &header, sizeof(header));
	DWORD size = this->header.info.biSizeImage;
	frames[0].frame = new BYTE[size];
	frames[1].frame = new BYTE[size];
	memset(frames[1].frame, 0xFF, size);
	index = 0;
	return &this->header;
}

DWORD Uncompresser::UnCompress(Frame * pSrcFrame, std::fstream& file,
		Frame * pDesFrame, DWORD size) {
	DWORD type = 0;
	DWORD len = 0;
	DWORD cur = 0;
	DWORD* pSrc = (DWORD*) pSrcFrame->frame;
	DWORD* pDes = (DWORD*) pDesFrame->frame;
	pDesFrame->x = pSrcFrame->x;
	pDesFrame->y = pSrcFrame->y;
	pDesFrame->type = pSrcFrame->type;
	while (cur != size) {
		len = 0;
		if (file.eof()) {
			return 1;
		}
		file.read((char*) &type, sizeof(type));
		if (type == TYPE_CURSOR) {
			file.read((char*) &(pDesFrame->x), sizeof(pDesFrame->x));
			file.read((char*) &(pDesFrame->y), sizeof(pDesFrame->y));
			file.read((char*) &(pDesFrame->type), sizeof(pDesFrame->type));
			continue;
		} else if (type == TYPE_PIXEL_OLD) {
			file.read((char*) &len, sizeof(len));
			//与上一幅画面相同的像素
			memcpy(pDes + cur, pSrc + cur, len * 4);
		} else if (type == TYPE_PIXEL_CPY) {
			file.read((char*) &len, sizeof(len));
			//变化的且与以前不同的像素
			file.read((char*) (pDes + cur), len * 4);
		} else if (type == TYPE_PIXEL_SAM) {
			file.read((char*) &len, sizeof(len));
			//同一个像素
			file.read((char*) &type, sizeof(type));
			for (DWORD i = 0; i < len; i++) {
				*(pDes + cur + i) = type;
			}
		} else {
			return type;
		}
		cur += len;
	}
	return 0;
}

Frame* Uncompresser::ReadFrame() {
	DWORD size = this->header.info.biWidth * this->header.info.biHeight;
	DWORD ok = UnCompress(&(frames[(index + 1) % 2]), file,
			&(frames[index % 2]), size);
	if (ok == 0) {
		index++;
		return &(frames[(index + 1) % 2]);
	} else {
		return NULL;
	}
}

void Uncompresser::Close() {
	delete frames[0].frame;
	delete frames[1].frame;
	file.close();
}
