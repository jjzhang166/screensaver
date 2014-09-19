/*
 * FileUncompressStream.cpp
 *
 *  Created on: 2014Äê9ÔÂ19ÈÕ
 *      Author: zhangbo
 */

#include "FileUncompressStream.h"

#include "include/zconf.h"
#include "include/zlib.h"
#define CHUNK 16384

FileUncompressStream::FileUncompressStream() {
}

FileUncompressStream::~FileUncompressStream() {
}

int FileUncompressStream::Open(const char* file) {
	return 0;
}

int FileUncompressStream::Read(char* buffer, int count) {
	return 0;
}

int FileUncompressStream::Close() {
	return 0;
}
