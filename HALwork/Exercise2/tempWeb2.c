#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>
#define BUF_SIZE 1024

int main(int argc, char *argv[]) {
  int fd, num_wr,fd3;
  char buf[1024];
    fd = open("https://www.google.com/",O_RDONLY);
while(1) {
//sprintf(buf2,"<html><body><h1>Temperatur: %lf grader</h1></body></html>",temp);

fopen("https://www.google.com/","r");
num_wr = read(fd,buf,strlen(buf));
printf("%s\n",buf);
sleep(1);
}
close(fd);
}

