/*
 * Compresser.h
 *
 *  Created on: 2014Äê8ÔÂ25ÈÕ
 *      Author: zhangbo
 */

#ifndef COMPRESSER_H_
#define COMPRESSER_H_

#include "IWriter.h"

class Compresser : public IWriter {

public:
	Compresser(const char* file);

	virtual ~Compresser();

	virtual Frame* Open(unsigned int w, unsigned int h);
	virtual void Write(void* data, unsigned int size);
	virtual Frame* WriteFrame(Frame* frame);
	virtual void Close();
};

#endif /* COMPRESSER_H_ */
