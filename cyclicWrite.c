#include "gen.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cyclicBuf.h"

int main() 
{
	int fd = shm_open("shared_buffer", O_CREAT | O_RDWR, S_IRWXU);

	if (fd == -1) 
	{
		perror("Can not open the file");
		return -1;
	}

	int result;
	result = ftruncate(fd, sizeof(struct cyclic_buf));
	if (result != 0) 
	{
		perror("Can not truncate the file");
		return result;
	}

	struct cyclic_buf* mem = mmap(NULL, sizeof(struct cyclic_buf),
		                        PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (mem == NULL) 
	{
		perror("mmap");
		return -1;
	}

	mem->pos = 0;
	uint32_t sd = 0;
	while (1) 
	{
		generate((void*)mem->buff[mem->pos].array, sd);
		sd++;
		mem->pos++;
	}
	
	return 0;
}
