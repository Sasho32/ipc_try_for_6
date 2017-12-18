#ifndef _CYCLICBUF
#define _CYCLICBUF
#define _SIZE 4096

#include <stdint.h>
struct chunk_t {
  volatile int array[_SIZE];
};

struct cyclic_buf {
  volatile uint64_t pos;
  volatile struct chunk_t buff[1000];
};

#endif

