// by RAM, 2022
// Inspired mainly by devmem, the peekpoke example and some others from internet

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

typedef unsigned __int128 uint128_t;

int main(int argc, char *argv[]) {
  int       i, fd;
  void      *ptr;
  uint128_t data[4];
  unsigned  addr, page_addr, page_size = sysconf(_SC_PAGESIZE);

  if (argc != 3) {
    printf("usage: %s DEV ADDR\n", argv[0]);
    printf("\n");
    printf("* DEV should be /dev/mem or /dev/uioX\n");
    printf("* ADDR should be specified as hex values\n");
    exit(-1);
  }

  fd = open(argv[1], O_RDONLY | O_SYNC);
  if (fd<1) {
    perror(argv[0]);
    printf("Invalid device '%s'\n", argv[1]);
    exit(-1);
  }

  addr = strtoul(argv[2], NULL, 0);

  //---------------------------------------------------------------------------
  // Here we have a big difference between /dev/mem and /dev/uioX
  //---------------------------------------------------------------------------
  page_addr = (addr & ~(page_size-1)); // It is used when /dev/mem is selcted
  if (strcmp(argv[1], "/dev/mem"))
    page_addr = 0; // It is used when /dev/uioX is selcted
  //---------------------------------------------------------------------------

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
    printf(                                             \
      "0x%08x (diff = %d)\n",                           \
      (uint32_t)data[i],                                \
      (i>0) ? (uint32_t)(data[i]-data[i-1]) : 0         \
    );

  //---------------------------------------------------------------------------

  printf("* 64-bits\n");

  for (i=0; i<4; i++)
    data[i] = *(volatile uint64_t*)ptr;
  for (i=0; i<4; i++)
    printf(                                             \
      "0x%08x%08x (diff = %d)\n",                       \
      (uint32_t)(data[i]>>32),                          \
      (uint32_t)data[i],                                \
      (i>0) ? (uint32_t)(data[i]-data[i-1]) : 0         \
    );

  //---------------------------------------------------------------------------

  printf("* 128-bits\n");

  for (i=0; i<4; i++)
    data[i] = *(volatile uint128_t*)ptr;
  for (i=0; i<4; i++)
    printf(                                             \
      "0x%08x%08x%08x%08x (diff = %d)\n",               \
      (uint32_t)(data[i]>>96), (uint32_t)(data[i]>>64), \
      (uint32_t)(data[i]>>32), (uint32_t)data[i],       \
      (i>0) ? (uint32_t)(data[i]-data[i-1]) : 0         \
    );

  //---------------------------------------------------------------------------

  printf("* The End\n");

  munmap(ptr, page_size);
  close(fd);

  return 0;
}
