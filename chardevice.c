#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/uaccess.h>

//定义设备结构体

struct chardevice{
    dev_t devid;//设备号
    int major; //主设备号 
    int minor; //次设备号
    struct cdev cdev;//字符设备注册
    struct class *class;//字符类 在此类下进行设备创建
    struct device *device;//设备类 用于创建节点
};

static struct chardevice mychardevice;

#define DEVICE_NAME "chardevice"

static char readbuffer[100];
static char writebuffer[100];

static char kernel_data[] = {"kernel data \n"};

int chardevice_open(struct inode *inode, struct file *filp)
{
    //printk("chardevice_open\n");
    filp->private_data = &mychardevice;
    return 0;
}

int chardevice_release(struct inode *inode, struct file *filp)
{
    //printk("chardevice_release\n");
    return 0;
}


ssize_t chardevice_read(struct file *filp, char __user *buffer, size_t count, loff_t *offset)
{
    int ret;
    
    memcpy(readbuffer,kernel_data,sizeof(kernel_data));
    ret = copy_to_user(buffer,readbuffer,count);
    if (ret < 0)
    {
        printk("chardevice_read kernel_data failed\n");
    }
    
    

    return 0;
}

ssize_t chardevice_write(struct file *filp, const char __user *buffer, size_t count, loff_t *offset)
{
    int ret;
    
    ret = copy_from_user(writebuffer,buffer,count);
    if (ret < 0)
    {
        printk("chardevice_write to kernel failed\n");
    }
    else
    {
        printk("kernel get data :%s\n",writebuffer);
    }
    

    return 0;
}



static struct file_operations mychardevice_fops = {
    .owner = THIS_MODULE,
    .open = chardevice_open,
    .read = chardevice_read,
    .write = chardevice_write,
    .release = chardevice_release,
};



static int __init chardevice_init(void)
{
    int rv = 0;
    //printk("chardevice_init\n");
    if(mychardevice.major)//如果已经指定了主设备号
    {
        printk("user order major\n");
        mychardevice.devid = MKDEV(mychardevice.major,0);//合并成设备号
        rv = register_chrdev_region(mychardevice.devid,1,DEVICE_NAME);  //注册设备号   
    }
    else
    {
        printk("use alloc major\n");
        rv = alloc_chrdev_region(&mychardevice.devid,0,1,DEVICE_NAME);
        mychardevice.major = MAJOR(mychardevice.devid);
        mychardevice.minor = MINOR(mychardevice.devid);
    }

    if (rv < 0)
    {
        printk("device id get fail\n");
    }

    mychardevice.cdev.owner = THIS_MODULE;
    
    cdev_init(&mychardevice.cdev,&mychardevice_fops);//初始化cdev结构体
    cdev_add(&mychardevice.cdev,mychardevice.devid,1);//添加设备

    mychardevice.class = class_create(THIS_MODULE,DEVICE_NAME);//创建一个类
    if (IS_ERR(mychardevice.class))
		return PTR_ERR(mychardevice.class);
    //printk("device_create\n");
    mychardevice.device = device_create(mychardevice.class,NULL,mychardevice.devid,NULL,DEVICE_NAME);//创建设备节点
    if(IS_ERR(mychardevice.device))
        return PTR_ERR(mychardevice.device);

    return 0;
}


static void __exit chardevice_exit(void)
{
    //printk("chardevice_exit\n");
    device_destroy(mychardevice.class,mychardevice.devid);//删除设备节点
    class_destroy(mychardevice.class);//删除类
    cdev_del(&mychardevice.cdev);//删除字符设备
    unregister_chrdev_region(mychardevice.devid,1);//注销设备号

}


module_init(chardevice_init);
module_exit(chardevice_exit);



MODULE_AUTHOR("alpha");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("a char device test");


