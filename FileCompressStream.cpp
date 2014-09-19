/*
 * FileCompressStream.cpp
 *
 *  Created on: 2014Äê9ÔÂ19ÈÕ
 *      Author: zhangbo
 */

#include "FileCompressStream.h"


FileCompressStream::FileCompressStream(): output(Z_NULL) {
}

FileCompressStream::~FileCompressStream() {
}

int FileCompressStream::Open(const char* file, int level) {
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    int ret = deflateInit(&strm, level);
    output = fopen(file, "wb");
    if (ret != Z_OK)
        return ret;
	return ret;
}

int FileCompressStream::Write(unsigned char* buffer, int count) {
	unsigned int have = 0;
    strm.avail_in = count;
    if (strm.avail_in == 0)
        return 0;
    strm.next_in = buffer;

    /* run deflate() on input until output buffer not full, finish
       compression if all of source has been read in */
    do {
        strm.avail_out = CHUNK;
        strm.next_out = outbuf;
        deflate(&strm, Z_NO_FLUSH);    /* no bad return value */
        have = CHUNK - strm.avail_out;
        if (fwrite(outbuf, 1, have, output) != have || ferror(output)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
    } while (strm.avail_out == 0);

	return 0;
}

int FileCompressStream::Close() {
	unsigned int have = 0;
    strm.avail_in = 0;
    strm.next_in = NULL;

    /* run deflate() on input until output buffer not full, finish
       compression if all of source has been read in */
    do {
        strm.avail_out = CHUNK;
        strm.next_out = outbuf;
        deflate(&strm, Z_FINISH);    /* no bad return value */
        have = CHUNK - strm.avail_out;
        if (fwrite(outbuf, 1, have, output) != have || ferror(output)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
    } while (strm.avail_out == 0);
    deflateEnd(&strm);
	fclose(output);
	return 0;
}
