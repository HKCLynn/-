/**
*@file Deal.h
*@author 徐英杰（541223130@qq.com）
*/

#ifndef DEAL_H 
#define DEAL_H 
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

float deg2rad(float deg);
float rad2deg(float rad);


struct ResultPnP
{
    double yaw;
    double pitch;
    double roll;
    Mat tran_vec;
    Mat rotat_vec;
    float distance;
};

#endif

