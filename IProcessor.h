/*
 * IProcessor.h
 *
 *  Created on: 2014��9��9��
 *      Author: zhangbo
 */

#ifndef IPROCESSOR_H_
#define IPROCESSOR_H_

#include <fstream>

class IProcessor {
public:
	IProcessor(const char* file);
	virtual ~IProcessor();

	virtual void* Open(unsigned int w, unsigned int h) = 0;
	virtual void Write(void* data, unsigned int size) = 0;
	virtual void WriteCursor(long x, long y) = 0;
	virtual void* WriteFrame(void* frame) = 0;
	virtual void Close(void* frame) = 0;

protected:
	unsigned int w;//���
	unsigned int h;//�߶�
	char file[256];
	std::fstream fout;
	unsigned char* lpData[2];

};

#endif /* IPROCESSOR_H_ */
