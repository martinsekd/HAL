#include <error.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>

int main()
{
    int fd, temp, datalen, err;
    char numb[128];

    while(1)
    {
        fd = open("/dev/i2c-1", O_RDONLY);
        ioctl(fd,0x0703,0x48) // i2cdev sys call;
        temp = read(fd, numb, 2);
        close(fd);
        printf("%d,%d C\n",numb[0],numb[1]);

        sleep(1);
    }
}
