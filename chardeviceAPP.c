#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <string.h>

static const char * version = "1.0";
static char *       devicename = "/dev/chardevice";

//getopt函数参数    单个字符 表示没有参数  如-a
//                 单个字符加：表示选项后必须跟一个参数 如-b 100 或者-b100 
///////////////////单个字符加：： 表示选项后可以有，也可以无参数
//短选项参数只支持解析如-d这种短参数 不能解析--d这种参数
static const char short_options[] = "d:rw:h";//表示-d 选项后必须有一个参数 -r选项后没有参数 -w选项后必须要有参数 -h后没有参数

static const struct option long_options[] = {
    {"device", required_argument, NULL, 'd'},
    {"read", no_argument, NULL, 'r'},
    {"write", required_argument, NULL, 'w'},
    {"help", no_argument, NULL, 'h'},
    {0, 0,0,0},
};

static void usage(FILE *fp,int argc,char **argv)
{
    fprintf(fp,
            "\nUsage: %s [Option]\n\n"
            "Version: %s\n"
            "Options:\n"
            "-d | --device name char device name: %s\n"
            "-r | --read kernel data\n"
            "-w | --write data to kernel. such as 12345\n"
            "-h | --help print this message\n"
            "",argv[0],version,devicename);
}

int device_open(const char *pathname)
{
    int fd = 0;
    fd = open(pathname,O_RDWR);
    if (fd < 0)
    {
        printf("open %s failed\n",pathname);
    }
    return fd;
}

void device_read_data(const char *pathname)
{
    int fd = 0;
    ssize_t ret = 0;
    char buffer[50]={0};
    fd = device_open(pathname);

    ret = read(fd, buffer, 20);
    if(ret < 0)
    {
        printf("read %s failed\n",pathname);
        
    }
    else
    {
        printf("user get data: %s\n",buffer);
    }
    
    close(fd);   
}


void device_write_data(const char *pathname,char * buffer)
{
    int fd = 0;
    ssize_t ret = 0;
    fd = device_open(pathname);
    ret = write(fd, buffer, strlen(buffer));
    if(ret < 0)
    {
        printf("write %s failed\n",pathname);
        
    }
    

}

int main(int argc, char **argv)
{

    

    if (argc < 2)//至少需要两个参数
    {
        usage(stdout,argc,argv);
        exit(EXIT_SUCCESS);
    }

    for(;;)
    {
        int opt_nxt;
        //long_options 指明了长参数的属性 longindex 如果为非空，则表示在long_options中找到的参数下表号
        opt_nxt = getopt_long(argc, argv, short_options, long_options, NULL);
        if (opt_nxt < 0)//如果等于-1，则表示遍历完所有的参数，没有找到目标参数
            break;
        switch(opt_nxt)
        {
            case 0:
            break;
            case 'd':
                devicename = optarg;
            break;

            case 'r':
                device_read_data(devicename);
            break;

            case 'w':
                device_write_data(devicename,optarg);
            break;

            case 'h':
                usage(stdout,argc,argv);
                exit(EXIT_SUCCESS);

            default:
            usage(stderr,argc,argv);
            exit(EXIT_FAILURE);
        }  
    }



    
    return 0;
}






