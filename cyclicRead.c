#include "gen.h"
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "cyclicBuf.h"


int main() 
{
	int fd = shm_open("shared_mem", O_RDONLY, 0);
	if (fd == -1) 
	{
		perror("Can not open file");
		return fd;
	}

	struct cyclic_buf* mem = mmap(NULL, sizeof(struct cyclic_buf), PROT_READ, MAP_SHARED, fd, 0);
	  
	if (mem == MAP_FAILED) 
	{
		perror("mmap");
		return -1;
	}

	uint64_t current_pos = 0;
	uint64_t previous = -1;

	printf("starting position: %ld\n", current_pos);

	bool checker = false;
	while (1) 
	{
		if (mem->pos == current_pos) 
		{
			sleep(1);
			continue;
		}

		if(mem->pos >= current_pos + 1000)
		{
			printf("out of bounds");
			break;
		}

		int64_t curr_seed = verify((void*)mem->buff[current_pos].array);

		if (curr_seed == -1 || (previous + 1 != curr_seed && checker == true)) 
		{
			break;
		}

		previous = curr_seed;

		printf("%ld -> %ld\n", current_pos, curr_seed);
		current_pos++;
	}

	return 0;
}
