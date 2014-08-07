#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define DEVICE "/dev/watchdog"
#define SENSOR_NUM 5

int main(int argc, char **argv)
{
	int fd;

	if ((fd = open(DEVICE, O_RDONLY)) < 0) {
		fprintf(stderr, "open(%s) = %d failed !\n", DEVICE, fd);
		perror("open(/dev/input-sensor) failed");
		exit(1);
	}

	printf("Watchdog is running\n");

	return 0;
}
