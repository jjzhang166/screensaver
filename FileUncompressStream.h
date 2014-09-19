/*
 * FileUncompressStream.h
 *
 *  Created on: 2014��9��19��
 *      Author: zhangbo
 */

#ifndef FILEUNCOMPRESSSTREAM_H_
#define FILEUNCOMPRESSSTREAM_H_

class FileUncompressStream {
public:
	FileUncompressStream();
	virtual ~FileUncompressStream();

	int Open(const char* file);
	int Read(char* buffer, int count);
	int Close();
};

#endif /* FILEUNCOMPRESSSTREAM_H_ */
