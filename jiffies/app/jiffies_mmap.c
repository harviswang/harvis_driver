#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
/*
 * System.map/AT91SAM9G45
 * 31217 c0512858 D jiffies
 * 31218 c0512858 D jiffies_64
 *
 * CentOS 6.5/System.map-2.6.32-431.el6.i686
 * 45172 c0a7ea00 D jiffies
 * 45173 c0a7ea00 D jiffies_64
 */
static int page_size;
#ifndef PAGE_SIZE
#define PAGE_SIZE (page_size)
#define PAGE_MASK (~(PAGE_SIZE - 1))
#endif

#define AT91SAM9G45	1

int main()
{
		int fd;
		char *p = NULL;
		int i;
#ifdef AT91SAM9G45	
		unsigned long offset = 0xc0512838;/* I got it from printk(&jiffies) in a kernel module*/
#else
		unsigned long offset = 0xc0a7ea00;
#endif	
		volatile unsigned long jiffies;

		fd = open("/dev/mem", O_RDONLY);
		if (fd < 0) {
				perror("open(%s) failed");
				return errno;
		}

		page_size = getpagesize();
		printf("PAGE_SIZE: %ld\n", PAGE_SIZE);
		printf("before offset -= 0xc0000000;\n");
		printf("%8lx\n", offset & PAGE_MASK);
		printf("%8lx\n", offset % PAGE_SIZE);
		offset -= 0xc0000000;
		printf("after offset -= 0xc0000000;\n");
		printf("%8lx\n", offset & PAGE_MASK);
		printf("%8lx\n", offset % PAGE_SIZE);
		printf("offset: %lx\n", offset);
		p = (char *)mmap(NULL, PAGE_SIZE, PROT_READ, MAP_SHARED, fd, (off_t)(offset & PAGE_MASK));
		if (p == MAP_FAILED)
				perror("mmap failed");

		for (i = 0 ; i < 5; i++) {
			jiffies = *((volatile unsigned long*)((char *)p + offset % PAGE_SIZE));
			printf("jiffes = %ld\n", jiffies);
		}
		return 0;
}
