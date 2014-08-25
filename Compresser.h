/*
 * Compresser.h
 *
 *  Created on: 2014年8月25日
 *      Author: zhangbo
 */

#ifndef COMPRESSER_H_
#define COMPRESSER_H_

#include <fstream>

class Compresser {
private:
	unsigned int w;//宽度
	unsigned int h;//高度
	std::fstream fout;
	unsigned char* lpData[2];

public:
	Compresser(unsigned int w, unsigned int h);

	virtual ~Compresser();

	virtual void* Open(const char* file);
	virtual void Write(void* data, unsigned int size);
	virtual void WriteCursor(long x, long y);
	virtual void* WriteFrame(void* frame);
	virtual void Close(void* frame);
};

#endif /* COMPRESSER_H_ */
