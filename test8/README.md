# 基础任务2

### 项目介绍
    通过darknet的框架用yolov4训练属于自己的神经网络的准备过程

### 具体过程

##### 下载安装darknet
1. 从github上下载darknet
    * $ git clone http://hub.fastgit.org/AlexeyAB/darknet
    * $ cd darknet
    * $ make

2. 将darknet文件夹下Makefile文件的opencv=0改为opencv=1 再次make。
##### 数据准备
1. 从官网上下载DJI数据集，并取出一部分的xml文件和对应的jpg文件。

2. 制作names文件，我选择的分类名有5个，按顺序一个一行分别为car(0)，watcher(1)，base(2)，ignore(3)，armor(4)。

3. 将每一个xml文件转换成txt文件(通过python写脚本实现)，txt文件每行代表一个obj的信息，分别为分类序号 中心x/图像宽度 中心y/图像高度 框宽/图像宽度 框高/图像高度

4. 将jpg文件和txt文件放到一起作为数据集

5. 制作train.txt文件，通过python脚本将所有的jpg文件路径写进train.txt文件中，再拷贝一份改名为test.txt文件

6. 在darknet文件夹下找到yolov4-custom.cfg文件，打开后修改以下参数
    * batch改为16
    * max_batch改为10000
    * steps改为8000，9000
    * 三个[yolo]-layers前的filters改为30

7. 下载yolov4.conv.137(预权重文件)。

8. 制作data文件，其中存入
    * classes=5
    * train=train.txt的文件路径
    * valid=test.txt的文件路径
    * names=names文件的路径
    * backup=训练生成的weight文件路径(自己定)

9. 以下是我的data文件
    * classes=5
    * train=/home/lynn/YOLO/train.txt
    * valid=/home/lynn/YOLO/test.txt
    * names=/home/lynn/YOLO/obj.names
    * backup=/home/lynn/YOLO/backup

##### 下载安装cuda

1. 由于知道了自己的显卡型号，在nivida官网查到可以使用的cuda，选择了cuda11.0下载安装

* $ wget http://developer.download.nvidia.com/compute/cuda/11.0.2/local_installers/cuda_11.0.2_450.51.05_linux.run
* $ sudo chmod +x cuda_11.0.3_450.51.06_linux.run
* $ sudo ./cuda_11.0.3_450.51.06_linux.run

* 注意 安装时不能勾选driver

2. 配置环境变量

    * $ vi ~/.bashrc

    * 加入
        export PATH="/usr/local/cuda-11.0/bin:$PATH"
        export LD_LIBRARY_PATH="/usr/local/cuda-11.0/lib64:$LD_LIBRARY_PATH"

    * $ source ~/.bashrc

3. 将darknet里面的Makefile中gpu对应的路径改为下载的cuda-11.0对应的路径

4. 将gpu=0改为gpu=1，make一次

##### 下载安装cudnn

1. 在nivida官网cuda11.0对应的cudnn为8.0，下载安装压缩包文件

2. 解压缩 $ tar zxvf xxxxx 之后得到的文件夹为cuda

3. 将主目录中cuda文件夹里的文件拷贝进 usr/local/cuda中
    * $ sudo cp -r cuda/include/* /usr/local/cuda/include/
    * $ sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/
    * $ sudo chmod a+r /usr/local/cuda/include/cudnn.h
    * $ sudo chmod a+r /usr/local/cuda/lib64/libcudnn*

4. 将darknet中的Makefile文件中cudnn=0改为cudnn=1 并make一次

##### 训练模型
1. 在darknet文件夹用终端打开
    $ ./darknet detector train data文件路径 cfg文件路径 预权重文件路径
2. 以下是我的命令
    $ ./darknet detector train /home/lynn/YOLO/obj.data /home/lynn/YOLO/yolov4-custom.cfg /home/lynn/YOLO/yolov4.conv.137

