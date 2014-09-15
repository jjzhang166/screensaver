/*
 * IWriter.cpp
 *
 *  Created on: 2014Äê9ÔÂ11ÈÕ
 *      Author: zhangbo
 */

#include "IWriter.h"
#include <string.h>

IWriter::IWriter(const char* file) : w(0), h(0) {
	if (strlen(file) < 256) {
		strcpy(this->file, file);
	}
}

IWriter::~IWriter() {
}

