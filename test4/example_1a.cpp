#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>

int main(int argc, char **argv)
{
  ros::init(argc, argv, "example1a");//初始化节点和节点名称
  ros::NodeHandle n;//节点相关进程的处理程序，实现节点与环境的交互
  ros::Publisher pub = n.advertise<std_msgs::String>("message", 100);//发布程序
  ros::Rate loop_rate(10);//设置发送频率
 
  while (ros::ok())//当按下Ctrl+C时停止
  {
    std_msgs::String msg;//创建消息变量
    std::stringstream ss;//创建消息流
    ss << "Hello World!I'm a boy";
    msg.data = ss.str();//赋值
    pub.publish(msg);//发布信息
    
    ros::spinOnce();//处理内部事件
    loop_rate.sleep();//休眠
  }
  return 0;
}

