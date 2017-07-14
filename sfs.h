#ifndef _LIBC_SFS
#define _LIBC_SFS

#include <stdint.h>

uint64_t sfs_open(char * path);
int sfs_read(size_t file_offset, uint8_t * buf, size_t count);
int sfs_mount(char* path);

#define VOLUMEID		0x01
#define START_MARKER	0x02
#define UNUSED_ENTRY	0x10
#define DIRECTORY_ENTRY	0x11
#define FILE_ENTRY		0x12
#define UNUSABLE_ENTRY	0x18
#define	DELETED_FLAG	0x08
#define CONTINUATION	0x20

void print_pretty(uint8_t * buf, int nbytes);

#endif
