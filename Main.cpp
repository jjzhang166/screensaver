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
	//return WinUtil::SaveScreenToRecordFile("Z:/Downloads/bmp02.records");
	//return WinUtil::BmpsToRecordFile("Z:/Downloads/bmps", "Z:/Downloads/bmp.records");
	//return WinUtil::RecordFileToBmps("Z:/Downloads/bmps01", "Z:/Downloads/bmp01.records");
	return WinUtil::RecordFileToAvi("Z:/Downloads/bmps.avi", "Z:/Downloads/bmp.records");
}
