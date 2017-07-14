#include <stdio.h>

#include "./sfs.h"

int main()
{
	sfs_mount("sfsimg.img");
	//int foffset = sfs_open("512B.txt");
	int foffset = sfs_open("deadbaba.txt");
	if(foffset)
	{
		printf("foffset: 0x%x\n", foffset);
		uint8_t buffer[512];
		sfs_read(foffset, buffer, 512);
		print_pretty(buffer, 512);
	}
	return 0;
}
