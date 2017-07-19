#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <inttypes.h>

#include "./sfs.h"

static int verbose_flag = 0;
int main(int argc, char **argv)
{
	static struct flags {
		unsigned int mount;
		unsigned int open;
		unsigned int read;
		unsigned int write;
		unsigned int close;
		unsigned int delete;
		unsigned int info;
	}f;
	char * imgpath = "sfsimg.img", * filepath = "deadbaba.txt";
	uint64_t foffset, to_read_bytes = 64;
	int c;
	while(1)
	{
		static struct option long_options[] = 
		{
			{"verbose", no_argument,	&verbose_flag, 1},
			{"brief",	no_argument,	&verbose_flag, 0},
			{"mount",	required_argument,	&f.mount,	'm'},
			{"open",	required_argument,	&f.open,	'o'},
			{"read",	required_argument,	&f.read,	'r'},
			{"write",	required_argument,	&f.write,	'w'},
			{"close",	required_argument,	&f.close,	'c'},
			{"delete",	required_argument,	&f.delete,	'd'},
			{"info",	no_argument,		&f.info,	'i'},
			{0, 0, 0, 0}
		};
		int option_index = 0;
		c = getopt_long(argc, argv, "m:o:r:w:c:d:i", long_options, &option_index);
		
		if (c == -1) break;
		
		switch(c)
		{
			case 0:
				switch(*long_options[option_index].flag){
				case 'm':
					imgpath = malloc(sizeof optarg);
					strcpy(imgpath, optarg);
					printf("image to mount: %s\n", imgpath);
					break;
				case 'o':
					filepath = strdup(optarg);
					printf("file to open: %s\n", filepath);
					break;
				case 'r':
					to_read_bytes = atoi(optarg);
					printf("Bytes to read: 0x%"PRIX64"\n", to_read_bytes);
					break;
				case 'w':case 'c':case 'd':case 'i':
					printf("Unimplemented\n");
				default:
					printf("Unrecognised\n");		
					return -1;
				}
		}
	}
	if(verbose_flag)
		printf("Verbose set. Output will be verbose. Everything that can be output, will be. The output may be very long, but this is implied by verbose.\n ");
	if(optind < argc)
	{
		printf("These options not processed: ");
		while(optind < argc)
			printf("%s ", argv[optind++]);
		putchar ('\n');
	}
	if(f.read)
		if(f.mount && f.open)
		{
			
			sfs_mount(imgpath);
			printf("Searching for file %s\n", filepath);
			foffset = sfs_open(filepath);
			if(foffset)
			{
				uint8_t buffer[to_read_bytes];
				sfs_read(foffset, buffer, to_read_bytes);
				print_pretty(buffer, to_read_bytes);
			} else printf("Failed to open file %s\n", filepath);
		}
		else
		{
			printf("Please mount an image and open a file\n");	
		}
	else
		printf("Only read files for now\n");
	return 0;
}
