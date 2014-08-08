#include <linux/miscdevice.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/gpio.h>
#include <linux/errno.h>


#define DEVICE_NAME "mymap"


static unsigned char array[10]={0,1,2,3,4,5,6,7,8,9};
static unsigned char *buffer;


static int my_open(struct inode *inode, struct file *file)
{
    return 0;
}


static int my_map(struct file *filp, struct vm_area_struct *vma)
{    
    unsigned long page;
    unsigned char i;
    unsigned long start = (unsigned long)vma->vm_start;
    //unsigned long end =  (unsigned long)vma->vm_end;
    unsigned long size = (unsigned long)(vma->vm_end - vma->vm_start + 1);

    //�õ������ַ
    page = virt_to_phys(buffer);    
    //���û��ռ��һ��vma�����ڴ���ӳ�䵽��page��ʼ��һ����������ҳ����
    if(remap_pfn_range(vma, start, page>>PAGE_SHIFT, size, PAGE_SHARED))//������������ҳ֡�ţ��������ַ����PAGE_SHIFT�õ�
        return -1;

    //�����ڴ�д10�ֽ�����
    for(i=0; i<10; i++)
        buffer[i] = array[i];
    
    return 0;
}


static struct file_operations dev_fops = {
    .owner  = THIS_MODULE,
    .open   = my_open,
    .mmap   = my_map,
};

static struct miscdevice misc = {
    .minor = MISC_DYNAMIC_MINOR,
    .name  = DEVICE_NAME,
    .fops  = &dev_fops,
};


static int __init dev_init(void)
{
    int ret;    

    //ע������豸
    ret = misc_register(&misc);
    //�ڴ����
    buffer = (unsigned char *)kmalloc(PAGE_SIZE, GFP_KERNEL);
	if (buffer == NULL)
		return -ENOMEM;
    //���ö��ڴ�����Ϊ����
    SetPageReserved(virt_to_page(buffer));

    return ret;
}


static void __exit dev_exit(void)
{
    //ע���豸
    misc_deregister(&misc);
    //�������
    ClearPageReserved(virt_to_page(buffer));
    //�ͷ��ڴ�
    kfree(buffer);
}


module_init(dev_init);
module_exit(dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("LKN@SCUT");

