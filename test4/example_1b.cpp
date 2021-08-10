#include "ros/ros.h"
#include "std_msgs/String.h"

//定义回调函数
void messageCallback(const std_msgs::String::ConstPtr& msg)
{
  ROS_INFO("Thanks: [%s]", msg->data.c_str());
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "example1b");
  //节点相关进程的处理程序，实现节点与环境的交互
  ros::NodeHandle n;
  //创建订阅器，并开始监听名为message的话题，设置缓冲区大小，处理消息的函数
  ros::Subscriber sub = n.subscribe("message", 1000, messageCallback);
  //主循环一直执行，按下Ctrl+C停止并结束
  ros::spin();
  return 0;
}

