/*
 * Recorder.h
 *
 *  Created on: 2014Äê9ÔÂ9ÈÕ
 *      Author: zhangbo
 */

#ifndef RECORDER_H_
#define RECORDER_H_

#include <cpputils/Runnable.h>
#include "IProcessor.h"

class Recorder : public Runnable {
protected:
	bool running;
	char file[256];
	IProcessor* processor;

public:
	Recorder(IProcessor* processor);
	virtual ~Recorder();

	void Run();

	void Stop();
};

#endif /* RECORDER_H_ */
