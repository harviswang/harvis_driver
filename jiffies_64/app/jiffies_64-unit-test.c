#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define DEVICE_NODE_NAME "/dev/jiffies_64"

int main(int argc, char **argv)
{
	unsigned long long jiffies_64; /* It's in userspace, so without trouble */
	int fd = -1;

	fd = open(DEVICE_NODE_NAME, O_RDONLY);
	if (fd == -1) {
		perror(DEVICE_NODE_NAME);
		return -1;
	}

	read(fd, &jiffies_64, sizeof(jiffies_64));
	printf("jiffies_64 = %lld\n", jiffies_64);
	
	close(fd);
	return 0;
}

