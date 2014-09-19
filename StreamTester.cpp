
#include "FileCompressStream.h"
#include "FileUncompressStream.h"

int test1() {
	FileCompressStream fcs;
	fcs.Open("a.zip", Z_DEFAULT_COMPRESSION);
	unsigned char c[] = "12312\r\n3123123123123123\r\n";
	for (int i=0; i<50; i++) {
		fcs.Write(c, 20);
	}
//	fcs.Write((unsigned char*)"123123123123123123\r\n", 20);
//	fcs.Write((unsigned char*)"123123123123123123123\r\n", 23);
	fcs.Close();

//	FileUncompressStream fus;
//	fus.Open("a.zip");
//	char buf[1024];
//	int len = fus.Read(buf, 1024);
//	Util::WriteFile("a.txt", buf);
//	fus.Close();

	return 0;
}
