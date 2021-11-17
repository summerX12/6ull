#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc,char **argv)
{
    int fd,err;
    unsigned char comm;

    if(argc < 2)
    {
        fprintf(stderr,"usage <0,1,2,4,7>\n");
        exit(0);
    }

    fd = open("/dev/rgb-leds",O_WRONLY);
    if(fd < 0)
    {
        perror("open()");
        exit(1);
    }

    comm = atoi(argv[1]);
    err = write(fd,&comm,sizeof(comm));
    if(err < 0)
    {
        perror("write()");
        close(fd);
        exit(1);
    }

    close(fd);

    exit(0);
}


