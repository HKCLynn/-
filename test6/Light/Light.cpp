/**
*@file Light.cpp
*@author 徐英杰（541223130@qq.com）
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include "Light.h"
#include"Deal.h"

using namespace std;
using namespace cv;

/**
*@brief 初始化灯条
*@param input_led 灯条外界四边形的四个点信息
*/
Light::Light(Point2f input_led[])
{
    //中间传输的灯条四个点信息
    Point2f led[4];
    for (int i = 0; i < 4; i++)
    {
        led[i] = input_led[i];
    }
    //排序
    sort(begin(led), end(led), [](Point2f &a, Point2f &b) -> bool
         { return a.y < b.y; });
    //灯条的上顶点
    top = (led[0] + led[1]) / 2;
    //灯条的下顶点
    bottom = (led[2] + led[3]) / 2;
    //灯条的高
    height = sqrt(pow(top.x - bottom.x, 2) + pow(top.y - bottom.y, 2));

}



