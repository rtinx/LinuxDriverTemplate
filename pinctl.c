
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#define	RTINX_IOCTL_MAGIC      'x'
#define	RTINX_IOCTL_GET_PIN    _IOR(RTINX_IOCTL_MAGIC, 1, unsigned int)
#define	RTINX_IOCTL_SET_PIN    _IOW(RTINX_IOCTL_MAGIC, 2, unsigned int)

int main(int argc, char *argv[])
{
    int fd_pinctl = -1, port = 0, pin = 0, value = 0;
    unsigned int command = 0;
	
	if (argc < 3)
	{
	    printf("Usage: pinctl <port> <pin> [value]\n");
		return -1;
	}
	
	fd_pinctl = open("/dev/pinctl", O_RDWR);
	if (fd_pinctl < 0)
	{
		printf("open /dev/pinctl failed, %d", errno);
	    return -1;
	}
	
	port = atoi(argv[1]);
	pin = atoi(argv[2]);
	
	if (argc == 4)
	{
		value = atoi(argv[3]);
	}
	
	command = ((port << 16) & 0xff0000) | ((pin << 8) & 0xff00) | (value & 0xff);
	
	if (argc == 4)
	{
	    ioctl(fd_pinctl, RTINX_IOCTL_SET_PIN, &command);
		printf("GPIO%d.%02d <- %d\n", port, pin, value);
	}
	else
	{
		ioctl(fd_pinctl, RTINX_IOCTL_GET_PIN, &command);
		printf("GPIO%d.%02d -> %d\n", port, pin, value);
	}
	
	close(fd_pinctl);
	
	return 0;
}
