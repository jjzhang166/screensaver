/*
 * WinUtil.h
 *
 *  Created on: 2014年8月18日
 *      Author: zhangbo
 */

#ifndef WINUTIL_H_
#define WINUTIL_H_

#include <windows.h>
#include <fstream>

class WinUtil {
public:
	WinUtil();
	virtual ~WinUtil();

	static int Create();
	static int RecordScreen(const char* file);
	static int PlayScreen(const char* file);

	static void Compress01(DWORD * pSrc, DWORD * pDes, std::fstream& file, DWORD size);

	static DWORD UnCompress01(DWORD * pSrc, std::fstream& file, DWORD * pDes, DWORD size);

	/**
	 * 每隔0.1秒将屏幕采集成BMP并保存在文件夹path下
	 * 保存的文件名为：000.bmp -> 100.bmp
	 */
	static int SaveScreenToBmps(const char* path);

	/**
	 * 每隔0.1秒将屏幕采集成BMP并保存在file中
	 */
	static int SaveScreenToRecordFile(const char* file);

	/**
	 * 将文件夹下的标准BMP文件，按照顺序压缩为Record文件。
	 */
	static int BmpsToRecordFile(const char* path, const char* file);

	/**
	 * 将Record文件，解压缩为标准的BMP文件。
	 */
	static int RecordFileToBmps(const char* path, const char* file);

	/**
	 * 将Record文件，解压缩为标准的AVI文件。
	 */
	static int RecordFileToAvi(const char* avi, const char* file);
};

#endif /* WINUTIL_H_ */
