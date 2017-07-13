#include <stdio.h>

#include "./sfs.h"

int main()
{
	sfs_mount("sfsimg.img");
	sfs_open("512B.txt", "r");
	return 0;
}
