# 进阶任务2

### 实现功能
    可以创建两个节点并进行通信

### 环境依赖
    Ubuntu18.04
    ROS Melodic

### 使用说明
1. 更改程序代码中视频路径，保证路径有效。
2. 建立工作空间
    $mkdir xxx/test4/src
    $cd xxx/test4/src
    $catkin_init_workspace
    $cd xxx/catkin_ws
    $catkin_make
3. 创建包
    $cd xxx/test4/src
    $catkin_create_pkg xxx(自己取名字) std_msgs roscpp
4. 编译包
    $cd xxx/test4
    $catkin_make
5. 将两个cpp文件放入test4/xxx/src中

6. 将工作空间路径加入到环境变量中
    $ source xxx/test4/devel/setup.bash  //添加环境变量

7.  $cd xxx/test4
    $catkin_make

8. 打开一个新终端
    $ roscore
9. 再打开两个新终端 分别运行
    $ rosrun learning_communication example1a
    $ rosrun learning_communication example1b
