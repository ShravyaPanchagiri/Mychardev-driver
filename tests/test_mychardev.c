#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define DEVICE "/dev/mychardev"

int main()
{
	int fd;
	char write_buf[100] = "Hello from user space!";
	char read_buf[100];

	fd = open(DEVICE, O_RDWR);
	if (fd < 0)
	{
		perror("Failed to open device");
		return 1;
	}

	printf("Writing to device: %s\n",write_buf);
	write(fd, write_buf, strlen(write_buf));

	lseek(fd, 0, SEEK_SET);
	read(fd, read_buf, sizeof(read_buf));
	printf("Read from device:%s\n", read_buf);

	close(fd);
	return 0;
}