/**
*@file Light.h
*@author 徐英杰（541223130@qq.com）
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;

class Light
{
public:
    //上顶点
    Point2f top;
    //下顶点
    Point2f bottom;
    //高
    double height;
    //初始化灯条
    Light(Point2f input_led[]);
};




