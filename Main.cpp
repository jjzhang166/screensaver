/*
 * update.cpp
 *
 *  Created on: 2012-11-1
 *      Author: zhangbo
 */

#include <windows.h>
#include <stdio.h>
#include <cpputils/Configure.h>
#include "WinUtil.h"
#include <cpputils/Util.h>
#include "include/zconf.h"
#include "include/zlib.h"

#define DIR "."

const char* BMPS_DIR = DIR "/bmps";
const char* BMPS_RECORD = DIR "/bmps.rec";
const char* BMPS_AVI = DIR "/bmps.avi";

int test1();
int test();
int WINAPI WinMain2(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
	test1();
	return test();
}

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
	Properties prop;
	Util::ParseCommandLine(cmdline, prop);

	string type = prop.Get("type", "RecordScreenToFile");

	if (type == "RecordScreenToFile") {
		return WinUtil::RecordScreenToFile(
				prop.Get("rec", BMPS_RECORD).c_str(),
				prop.GetInteger("count", 10));
	} else if (type == "SaveScreenToBmps") {
		Util::EnsureDir(prop.Get("dir", BMPS_DIR));
		return WinUtil::SaveScreenToBmps(prop.Get("dir", BMPS_DIR).c_str());
	} else if (type == "BmpsToRecordFile") {
		Util::EnsureDir(prop.Get("dir", BMPS_DIR));
		return WinUtil::BmpsToRecordFile(prop.Get("dir", BMPS_DIR).c_str(),
				prop.Get("rec", BMPS_RECORD).c_str());
	} else if (type == "RecordFileToBmps") {
		Util::EnsureDir(prop.Get("dir", BMPS_DIR));
		return WinUtil::RecordFileToBmps(prop.Get("dir", BMPS_DIR).c_str(),
				prop.Get("rec", BMPS_RECORD).c_str());
	} else if (type == "RecordFileToAvi") {
		return WinUtil::RecordFileToAvi(prop.Get("avi", BMPS_AVI).c_str(),
				prop.Get("rec", BMPS_RECORD).c_str(),
				prop.GetInteger("quality", 7500), prop.GetInteger("scale", 1),
				prop.GetInteger("rate", 10));
	}

	return 0;
}
