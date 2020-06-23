#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/ioctl.h>


int main(int argc, char *argv[]) {
  int fd, num_wr, fd2,fd3;
  int num_wr2;
  int ret=0;
  char data[2];
  char buf[128] = "out";
  char buf2[128];
  if(argc != 2) {
    printf("Fejl i argumenter");
    return -1;
  }
   char bufd[128];
   char bufv[128];
   //buf2 = argv[1];
   sprintf(bufd,"/sys/class/gpio/gpio%s/direction",argv[1]);
   sprintf(bufv,"/sys/class/gpio/gpio%s/value",argv[1]);

fd = open(bufd,O_WRONLY);
num_wr = write(fd,buf,strlen(buf));
  if (num_wr == -1) {
  printf("Fejl");
  return num_wr;
}
fd = open(bufv,O_WRONLY);
fd2 = open("/dev/i2c-1",O_RDWR);
ret = ioctl(fd2,0x0703,0x48);
if(ret < 0) {
printf("Fejl i2c\n");
} else {
printf("succes!\n");
}
//ret = read(fd,rddata,1);

while(1) {
  ret = read(fd2,data,2);
  double temp = data[0]+data[1]/1000;
printf("Temperatur: %lf\n",temp);

    if(temp>=30) {
    num_wr = write(fd,"1",1);
    fprintf(stdout,"ADVARSEL: Hoj temperatur\n");
    } else {
    num_wr = write(fd,"0",1);
    }
    sleep(1);
}
close(fd);
close(fd2);
}

