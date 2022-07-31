// by RAM, 2022
// Inspired by the peekpoke example and devmem

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

typedef unsigned __int128 uint128_t;

int main(int argc, char *argv[]) {
  int       i, fd;
  void      *ptr;
  uint128_t data[4];
  unsigned  addr, page_addr, page_offset;
  unsigned  page_size = sysconf(_SC_PAGESIZE);

  if (argc != 3) {
    printf("usage: %s DEV ADDR\n", argv[0]);
    printf("\n");
    printf("* DEV should be /dev/mem or /dev/uioX\n");
    printf("* ADDR may be specified as hex values\n");
    exit(-1);
  }

  fd = open(argv[1], O_RDONLY | O_SYNC);
  if (fd<1) {
    perror(argv[0]);
    printf("Invalid device '%s'\n", argv[1]);
    exit(-1);
  }

  addr        = strtoul(argv[2], NULL, 0);
  page_addr   = (addr & ~(page_size-1));
  page_offset = addr-page_addr;

  printf( \
    "ADDR=0x%08x PAGE_SIZE=0x%08x PAGE_ADDR=0x%08x PAGE_OFFSET=0x%08x\n\n", \
    addr, page_size, page_addr, page_offset \
  );

  ptr = mmap(NULL, page_size, PROT_READ, MAP_SHARED, fd, page_addr);
  if (ptr == MAP_FAILED) {
    perror(argv[0]);
    printf("MMAP failed\n");
    exit(-1);
  }

  //---------------------------------------------------------------------------

  printf("* 32-bits\n");

  for (i=0; i<4; i++)
    data[i] = *(volatile uint32_t*)ptr;
  for (i=0; i<4; i++)
    printf(                         \
      "0x%08x (diff = %0d)\n",      \
      (uint32_t)data[i],            \
      (i>0) ? data[i]-data[i-1] : 0 \
    );

  //---------------------------------------------------------------------------

  printf("* 64-bits\n");

  for (i=0; i<4; i++)
    data[i] = *(volatile uint64_t*)ptr;
  for (i=0; i<4; i++)
    printf(                         \
      "0x%08x%08x (diff = %0d)\n",  \
      (uint32_t)(data[i]>>32),      \
      (uint32_t)data[i],            \
      (i>0) ? data[i]-data[i-1] : 0 \
    );

  //---------------------------------------------------------------------------

  printf("* 128-bits\n");

  for (i=0; i<4; i++)
    data[i] = *(volatile uint128_t*)ptr;
  for (i=0; i<4; i++)
    printf(                                             \
      "0x%08x%08x%08x%08x (diff = %0d)\n",              \
      (uint32_t)(data[i]>>96), (uint32_t)(data[i]>>64), \
      (uint32_t)(data[i]>>32), (uint32_t)data[i],       \
      (i>0) ? data[i]-data[i-1] : 0                     \
    );

  //---------------------------------------------------------------------------

  printf("* The End\n");

  munmap(ptr, page_size);
  close(fd);

  return 0;
}
