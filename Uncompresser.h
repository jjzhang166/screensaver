/*
 * Uncompresser.h
 *
 *  Created on: 2014Äê9ÔÂ11ÈÕ
 *      Author: zhangbo
 */

#ifndef UNCOMPRESSER_H_
#define UNCOMPRESSER_H_

#include <fstream>
#include "Frame.h"

class Uncompresser {
public:
	Uncompresser();
	virtual ~Uncompresser();

	PBMPHEADER Open(const char* file);
	Frame* ReadFrame();
	void Close();

private:
	unsigned long UnCompress(Frame * pSrcFrame, std::fstream& file, Frame * pDesFrame,
			unsigned long size);

	std::fstream file;
	BMPHEADER header;
	Frame frames[2];
	unsigned long index;

};

#endif /* UNCOMPRESSER_H_ */
