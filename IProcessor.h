/*
 * IProcessor.h
 *
 *  Created on: 2014年9月9日
 *      Author: zhangbo
 */

#ifndef IPROCESSOR_H_
#define IPROCESSOR_H_

#include <fstream>

class IProcessor {
public:
	IProcessor(const char* file);
	virtual ~IProcessor();

	void SetSize(unsigned int w, unsigned int h);
	virtual void* Open() = 0;
	virtual void Write(void* data, unsigned int size) = 0;
	virtual void WriteCursor(long x, long y) = 0;
	virtual void* WriteFrame(void* frame) = 0;
	virtual void Close(void* frame) = 0;

protected:
	unsigned int w;//宽度
	unsigned int h;//高度
	char file[256];
	std::fstream fout;
	unsigned char* lpData[2];

};

#endif /* IPROCESSOR_H_ */
