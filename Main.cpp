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

#define DIR "."

const char* BMPS_DIR = DIR "/bmps";
const char* BMPS_RECORD = DIR "/bmps.rec";
const char* BMPS_AVI = DIR "/bmps.avi";

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmdline, int show) {
	vector<string> cmds = Util::Split(cmdline, ' ');
	Properties prop;
	for (unsigned int i = 0; i < cmds.size(); i++) {
		vector<string> nv = Util::Split(cmds[i], '=');
		if (nv.size() == 2) {
			prop.Put(nv[0], nv[1]);
		}
	}
	string type = prop.Get("type", "SaveScreenToRecordFile");

	if (type == "SaveScreenToRecordFile") {
		return WinUtil::SaveScreenToRecordFile(
				prop.Get("rec", BMPS_RECORD).c_str(),
				prop.GetInteger("count", 1000));
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
				prop.Get("rec", BMPS_RECORD).c_str(),
				prop.GetInteger("count", 1000));
	} else if (type == "RecordFileToAvi") {
		return WinUtil::RecordFileToAvi(prop.Get("avi", BMPS_AVI).c_str(),
				prop.Get("rec", BMPS_RECORD).c_str(),
				prop.GetInteger("quality", 7500), prop.GetInteger("scale", 1),
				prop.GetInteger("rate", 10),
				prop.GetInteger("count", 1000));
	}

	return 0;
}
