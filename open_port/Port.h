#include <iostream>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>

using namespace std;

//串口初始化
int set_opt(int fd, int nSpeed, int nBits, char nEvent, int nStop);
//打开串口
int open_port(int fd);
