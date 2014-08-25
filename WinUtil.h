/*
 * WinUtil.h
 *
 *  Created on: 2014��8��18��
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

	/**
	 * XXX01 ѹ���㷨��
	 * �Ƚϵ�ǰͼƬ����һ��ͼƬ�е���ͬ���ز��֣�ֻ�Ƚ���ͬλ�ã��������ͬ���¼��ͬ����������
	 * �Ƚϵ�ǰ�����Ƿ��ظ�������ظ���ֻ��¼һ���Լ����ظ�����
	 */
	static void Compress01(DWORD * pSrc, DWORD * pDes, std::fstream& file, DWORD size);
	static DWORD UnCompress01(DWORD * pSrc, std::fstream& file, DWORD * pDes, DWORD size);

	/**
	 * ÿ��0.1�뽫��Ļ�ɼ���AVI���������ļ���
	 */
	static int SaveScreenToAvi(const char* file, DWORD count);
	/**
	 * ÿ��0.1�뽫��Ļ�ɼ���BMP���������ļ���path��
	 * ������ļ���Ϊ��000.bmp -> 100.bmp
	 */
	static int SaveScreenToBmps(const char* path);

	/**
	 * ÿ��0.1�뽫��Ļ�ɼ���BMP��������file��
	 */
	static int RecordScreenToFile(const char* file, DWORD count);

	/**
	 * ÿ��0.1�뽫��Ļ�ɼ���BMP��������file��
	 */
	static int SaveScreenToRecordFile(const char* file, DWORD count);

	/**
	 * ���ļ����µı�׼BMP�ļ�������˳��ѹ��ΪRecord�ļ���
	 */
	static int BmpsToRecordFile(const char* path, const char* file);

	/**
	 * ��Record�ļ�����ѹ��Ϊ��׼��BMP�ļ���
	 */
	static int RecordFileToBmps(const char* path, const char* file, DWORD count);

	/**
	 * ��Record�ļ�����ѹ��Ϊ��׼��AVI�ļ���
	 */
	static int RecordFileToAvi(const char* avi, const char* filee, DWORD dwQuality, DWORD dwScale, DWORD dwRate, DWORD count);
};

#endif /* WINUTIL_H_ */
