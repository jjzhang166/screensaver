/*
 * update.cpp
 *
 *  Created on: 2012-11-1
 *      Author: zhangbo
 */

#include <windows.h>
#include <stdio.h>
#include "WinUtil.h"

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
	//return WinUtil::RecordScreen("./myout.bmp");
	return WinUtil::PlayScreen("./myout.bmp");
}

