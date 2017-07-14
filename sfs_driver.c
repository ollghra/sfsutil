#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "./sfs.h"

struct sfs_superblock {
	uint64_t timestamp;
	uint64_t data_size;
	uint64_t index_size;
	uint32_t Magic_version;
	uint64_t total_blocks;
	uint32_t reserved_blocks;
	uint8_t  block_size;
	uint8_t  checksum;
}__attribute__((__packed__)) SFS_Superblock;

FILE * image;

uint64_t reverse64(uint64_t n)
{
	uint64_t retval;
	uint8_t * np = (uint8_t *)&n;
	uint8_t * retvalp = (uint8_t *)&retval;
	for ( int i = 0; i<sizeof(uint64_t); i++)
		retvalp[i] = np[sizeof(uint64_t) -1 -i];
	return retval;
}
uint32_t reverse32(uint32_t n)
{
	    uint64_t retval;
		uint8_t * np = (uint8_t *)&n;
		uint8_t * retvalp = (uint8_t *)&retval;
		for ( int i = 0; i<sizeof(uint32_t); i++)
			retvalp[i] = np[sizeof(uint32_t) -1 -i];
		return retval;
}
int buffer_open(char * path)
{
	if((image = fopen(path, "rb+")) == 0)
		image = fopen(path, "wb+");
	if(image == 0)
	{
		printf("buffer fd == 0");
		return 1;
	}
	//uint8_t buf[0x193];
	//fread(buf, sizeof(uint8_t), 0x194, image);
	fseek(image, 0x194, SEEK_SET);
	
	uint64_t tmp64;
	uint32_t tmp32;
	uint8_t  tmp8;

	fread(&tmp64, sizeof(tmp64), 1, image);
	SFS_Superblock.timestamp = tmp64; 
	fread(&tmp64, sizeof(tmp64), 1, image);
	SFS_Superblock.data_size = tmp64;
	fread(&tmp64, sizeof(tmp64), 1, image);
	SFS_Superblock.index_size = tmp64;
	fread(&tmp32, sizeof(tmp32), 1, image);
	SFS_Superblock.Magic_version = tmp32;
	fread(&tmp64, sizeof(tmp64), 1, image);
	SFS_Superblock.total_blocks = tmp64;
	fread(&tmp32, sizeof(tmp32), 1, image);
	SFS_Superblock.reserved_blocks = tmp32;
	fread(&tmp8, sizeof(tmp8), 1, image);
	SFS_Superblock.block_size = tmp8;
	fread(&tmp8, sizeof(tmp8), 1, image);
	SFS_Superblock.checksum = tmp8;	
	return 0;
}

int buffer_read(size_t * buf, size_t count, size_t size, uint64_t offset)
{
	fseek(image, offset, SEEK_SET);
	fread(buf, size, count, image);
	return 0;
}

int sfs_mount(char * path)
{
	printf("Mounting SFS image %s\n", path);
	if(buffer_open(path))
		return 1;
	printf("Superblock:\ntimestamp: 0x%" PRIX64 "\ndata area: %" PRIu64 " blocks\nMagic?: %" PRIX32 "\n\
version: %" PRIX8 "\ntotal blocks: %" PRIu64 "\nreserved blocks: %" PRIu32 "\nblock size: %" PRIu16 "B\n\
checksum?: %s\n",
	SFS_Superblock.timestamp, SFS_Superblock.data_size, SFS_Superblock.Magic_version & 0xFFFFFF,
	(SFS_Superblock.Magic_version & 0xFF000000) >> (24), SFS_Superblock.total_blocks, SFS_Superblock.reserved_blocks, SFS_Superblock.block_size * 256,
	"Not yet implemented\0");
	return 0;
}

void print_pretty(uint8_t * buf, int nbytes)
{
	for(int i = 0; i < nbytes; i++)
	{
		for(int j = 0; j < 16; j++, i++)
		{
			if(j == 8)
			   	printf("  ");
			printf("%02" PRIX8 " ", buf[i]);
		}
		printf("\n");
	}

}
struct IndexEntry{
	uint8_t type;
};
int sfs_atos(uint8_t * buf, int len, char* s)
{
	for(int i = 0; buf[i] != 0 && i < len; i++)
		*(s + i) = buf[i];
	return 0;
}
int sfs_open(char * path, char * mode)
{
	unsigned int entry;
	struct IndexEntry index_entry;
	entry = SFS_Superblock.total_blocks << (SFS_Superblock.block_size + 7);
	uint8_t buf[64]; 
	do{
		entry -= 64;
		buffer_read((size_t *)buf, 64, sizeof(uint8_t), entry);
		index_entry.type = buf[0];
		print_pretty(buf, 64);
		if(index_entry.type == FILE_ENTRY)
		{
			char str[30];
			sfs_atos(&buf[0x22], 30, str);
			printf("File found:\nname: %s\n", str);
		}
	} while(index_entry.type != START_MARKER);	
	if(index_entry.type == START_MARKER)
		printf("No files found\n");
//	for(entry-=64;index_entry.typ

}
