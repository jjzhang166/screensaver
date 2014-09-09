/*
 * Compresser.h
 *
 *  Created on: 2014Äê8ÔÂ25ÈÕ
 *      Author: zhangbo
 */

#ifndef COMPRESSER_H_
#define COMPRESSER_H_

#include "IProcessor.h"

class Compresser : public IProcessor {

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
