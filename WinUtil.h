/*
 * WinUtil.h
 *
 *  Created on: 2014Äê8ÔÂ18ÈÕ
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
	static DWORD Compress(BYTE * pSrc, BYTE * pDes, BYTE * pOut, BYTE* pUn, DWORD size);
	static void Compress(BYTE * pSrc, BYTE * pDes, std::fstream& file, DWORD size);
	static DWORD UnCompress(BYTE * pSrc, BYTE * pIn, BYTE * pDes, BYTE* pUn, DWORD size);
	static int RecordScreen(const char* file);
	static int PlayScreen(const char* file);
};

#endif /* WINUTIL_H_ */
