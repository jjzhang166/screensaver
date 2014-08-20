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
	//return WinUtil::SaveScreenToBmps("Z:/Downloads/bmps");
	//return WinUtil::BmpsToRecordFile("Z:/Downloads/bmps", "Z:/Downloads/bmp.records");
	return WinUtil::RecordFileToBmps("Z:/Downloads/bmps01", "Z:/Downloads/bmp.records");
}
