/**
*@file Deal.h
*@author 徐英杰（541223130@qq.com）
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

//获得距离
double get_distance(Point2f point_one, Point2f point_two);
//图像处理
Mat imgPreprosses(const Mat src);



