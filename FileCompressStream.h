/*
 * FileCompressStream.h
 *
 *  Created on: 2014Äê9ÔÂ19ÈÕ
 *      Author: zhangbo
 */

#ifndef FILECOMPRESSSTREAM_H_
#define FILECOMPRESSSTREAM_H_

#include <stdio.h>
#include "include/zconf.h"
#include "include/zlib.h"
#define CHUNK 16384

class FileCompressStream {
public:
	FileCompressStream();
	virtual ~FileCompressStream();

	int Open(const char* file, int level);
	int Write(unsigned char* buffer, int count);
	int Close();

private:
	FILE* output;
    z_stream strm;
    unsigned char inbuf[CHUNK];
    unsigned char outbuf[CHUNK];
};

#endif /* FILECOMPRESSSTREAM_H_ */
