/*
 * Recorder.h
 *
 *  Created on: 2014Äê9ÔÂ9ÈÕ
 *      Author: zhangbo
 */

#ifndef RECORDER_H_
#define RECORDER_H_

#include <cpputils/Runnable.h>
#include "IWriter.h"

class Recorder : public Runnable {
protected:
	bool running;
	char file[256];
	IWriter* writer;

public:
	Recorder(IWriter* writer);
	virtual ~Recorder();

	void Run();

	void Stop();
};

#endif /* RECORDER_H_ */
