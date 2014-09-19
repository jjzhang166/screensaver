/*
 * IWriter.h
 *
 *  Created on: 2014��9��11��
 *      Author: zhangbo
 */

#ifndef IWRITER_H_
#define IWRITER_H_

#include "Frame.h"

class IWriter {
public:
	IWriter(const char* file);
	virtual ~IWriter();

	virtual Frame* Open(unsigned int w, unsigned int h) = 0;
	virtual void Write(void* data, unsigned int size) = 0;
	virtual Frame* WriteFrame(Frame* frame) = 0;
	virtual void Close() = 0;

protected:
	unsigned int w;//���
	unsigned int h;//�߶�
	char file[256];
	Frame frames[2];
};

#endif /* IWRITER_H_ */
