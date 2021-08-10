/*@file head.hpp*/
/*author 徐英杰（541223130@qq.com）*/


#include <iostream>
#include <opencv2/opencv.hpp>


using namespace std;
using namespace cv;

//确定左右灯条
void left_right(Point2f left[], Point2f right[]);
//画出某一灯条的上下两个点
void write_circle(Point2f led[], Mat frame);
//确定某一灯条的上下两个点
void top_bottom(Point2f &top, Point2f &bottom, Point2f *led);
//测距并且输出距离
void range_finding(float length, float weight, Point2f bottom_left, Point2f bottom_right, Point2f top_right, Point2f top_left);