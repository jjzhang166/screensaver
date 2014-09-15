/*
 * IReader.h
 *
 *  Created on: 2014��9��11��
 *      Author: zhangbo
 */

#ifndef IREADER_H_
#define IREADER_H_

#include "Frame.h"

class IReader {
public:
	IReader();
	virtual ~IReader();

	virtual PBMPHEADER Open(const char* file) = 0;
	virtual Frame* ReadFrame() = 0;
	virtual void Close() = 0;

};

#endif /* IREADER_H_ */
