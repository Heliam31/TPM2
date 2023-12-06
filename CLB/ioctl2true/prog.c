#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MY_CMD_MAGIC 'x'
#define MY_CMD_RESET _IO(MY_CMD_MAGIC,0)
#define BUFFER_IOCTL_STRLEN _IOR(BUFFER_IOCTL_MAGIC, 0, int)

int main() {
	int fd = open("/dev/deviceTest", O_RDWR), res;
	size_t sz;

	if (fd < 0) {
		perror("open()");
		return EXIT_FAILURE;
	}

	if ((res = ioctl(fd, MY_CMD_RESET)) != 0) {
		perror("ioctl()");
		return EXIT_FAILURE;
	}

	printf("bufsize: %lu\n", sz);

	close(fd);

	return EXIT_SUCCESS;
}
