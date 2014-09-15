/*
 * Frame.h
 *
 *  Created on: 2014��9��12��
 *      Author: zhangbo
 */

#ifndef FRAME_H_
#define FRAME_H_

#pragma pack (2)

typedef struct tagFrame {
	long time;//��ǰ֡�Ĳɼ�ʱ��
	long x;//���Xλ��
	long y;//���Yλ��
	long type;//�������ͣ���״��
	unsigned char* frame;//֡����
} Frame;

typedef struct tagBMP_FILE_HEADER {
	unsigned short bfType;
	unsigned long bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned long bfOffBits;
} BMP_FILE_HEADER,*LPBMP_FILE_HEADER;

typedef struct tagBMP_INFO_HEADER {
	unsigned long biSize;
	long biWidth;
	long biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned long biCompression;
	unsigned long biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	unsigned long biClrUsed;
	unsigned long biClrImportant;
} BMP_INFO_HEADER,*LPBMP_INFO_HEADER,*PBMP_INFO_HEADER;

typedef struct tagBMPHEADER {
	BMP_FILE_HEADER file;
	BMP_INFO_HEADER info;
} BMPHEADER,*LPBMPHEADER,*PBMPHEADER;

#endif /* FRAME_H_ */
