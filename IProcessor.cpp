/*
 * IProcessor.cpp
 *
 *  Created on: 2014Äê9ÔÂ9ÈÕ
 *      Author: zhangbo
 */

#include "IProcessor.h"
#include <string.h>

IProcessor::IProcessor(const char* file) : w(0), h(0) {
	if (strlen(file) < 256) {
		strcpy(this->file, file);
	}
}

void IProcessor::SetSize(unsigned int w, unsigned int h) {
	this->w = w;
	this->h = h;
}

IProcessor::~IProcessor() {
}

