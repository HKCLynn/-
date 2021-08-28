/**
*@file Deal.cpp
*@author 徐英杰（541223130@qq.com）
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include"Deal.h"

using namespace std;
using namespace cv;


/**
*@brief 获得距离
*@param point_one 其中一个点
*@param point_two 另一个点
*@return double 这两个点的距离
*/
double get_distance(Point2f point_one, Point2f point_two)
{
    double distance;
    distance = sqrt(pow(point_one.x - point_two.x, 2) + pow(point_one.y - point_two.y, 2));
    return distance;
}


/**
*@brief 图像处理
*@param src 输入图像
*@return Mat 处理后的图像
*/

Mat imgPreprosses(const Mat src)
{
    //分离得到的通道
    vector<Mat> channels;
    //图像分离通道
    split(src, channels);
    //通道相减后得到的图像
    Mat image;
    if (channels.size() == 3)
        image = channels[0] - channels[2];
    else
        image = src;
    //最后处理得到的图像
    Mat mask;
    threshold(image, mask, 100, 255, THRESH_BINARY); //阈值化
    blur(mask, mask, Size(3, 3));
    medianBlur(mask, mask, 3);
    //创建核
    Mat kernel = getStructuringElement(MORPH_ELLIPSE, Size(5, 5));
    //闭运算
    morphologyEx(mask, mask, MORPH_DILATE, kernel, Point(-1, -1), 1);
    return mask;
}



