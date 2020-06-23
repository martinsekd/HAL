#include <error.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd, num_wr;
    char buf1[128] = "1";
    char buf0[128] = "0";

    while(1)
    {
        fd = open("/sys/class/gpio/gpio26/value", O_WRONLY);
        num_wr = write(fd, buf1, strlen(buf1));
        sleep(1);

        fd = open("/sys/class/gpio/gpio26/value", O_WRONLY);
        num_wr = write(fd, buf0, strlen(buf0));
        sleep(1);
    }
}
