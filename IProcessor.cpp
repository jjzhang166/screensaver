/*
 * IProcessor.cpp
 *
 *  Created on: 2014��9��9��
 *      Author: zhangbo
 */

#include "IProcessor.h"
#include <string.h>

IProcessor::IProcessor(const char* file) : w(0), h(0) {
	if (strlen(file) < 256) {
		strcpy(this->file, file);
	}
}

IProcessor::~IProcessor() {
}

