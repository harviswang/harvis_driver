#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#define DEVICE "/dev/input-sensor"
#define SENSOR_NUM 5

int main(int argc, char **argv)
{
	int fd;
	int readbytes;
	int i, j = 0;
	char sensor_last_values[SENSOR_NUM] = {0, 0, 0, 0, 0};
	char sensor_current_values[SENSOR_NUM];

	if ((fd = open(DEVICE, O_RDONLY)) < 0) {
		fprintf(stderr, "open(%s) = %d failed !\n", DEVICE, fd);
		perror("open(/dev/input-sensor) failed");
		exit(1);
	}

	for (;;j++) {
		if (j == 2) {
			printf("------------------------------------\n");
			j = 0;
		}
		if ((readbytes = read(fd, sensor_current_values, SENSOR_NUM)) < 0) {
			fprintf(stderr, "read() failed\n");
			exit(2);
		}

		for (i = 0; i < readbytes; i++) {
			if (sensor_current_values[i] != sensor_last_values[i]) {
				printf("Sensor[%d] value changed from %d to %d\n", i, sensor_last_values[i], sensor_current_values[i]);
				sensor_last_values[i] = sensor_current_values[i];
			}
		}
	}

	return 0;
}
