#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include"Port.h"

using namespace std;


int main()
{
    //新建一个文件描述符
    int fd;
    //打开串口
    fd = open_port(fd);
    //串口初始化
    set_opt(fd, 115200, 7, 'O', 1);
    //数据内容
    char hello[10] = "hello";
    //向串口发送数据
    write(fd, hello, 10);
}