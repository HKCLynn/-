/**
 * @file Deal.h
 * @author 徐英杰（541223130@qq.com）
 */

#pragma once

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

//获得距离
double get_distance(Point2f point_one, Point2f point_two);
//图像处理
Mat imgPreprosses(const Mat src);
//用来获得相机中心相对于装甲板中心的角度
Point2f calculateRelativeAngle(const Mat &cameraMatrix, const Mat &distCoeff, Point2f center);
//角度转弧度
float deg2rad(float deg);
//弧度转角度
float rad2deg(float rad);
