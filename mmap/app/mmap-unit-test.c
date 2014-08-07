#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h> 

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

int main(int argc , char *argv[])
{
    int fd;
    int i;
    unsigned char *p_map;
    
    //���豸
    fd = open("/dev/mymap",O_RDWR);
    if(fd < 0)
    {
        printf("open fail\n");
        exit(1);
    }

    //�ڴ�ӳ��
    p_map = (unsigned char *)mmap(0, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,fd, 0);
    if(p_map == MAP_FAILED)
    {
        printf("mmap fail\n");
        goto here;
    }

    //��ӡӳ�����ڴ��е�ǰ10���ֽ�����
    for(i=0;i<10;i++)
        printf("%d\n",p_map[i]);
    

here:
    munmap(p_map, PAGE_SIZE);
    return 0;
}
