/**
*@file Light.h
*@author 徐英杰（541223130@qq.com）
*/


#pragma once
#include <stdio.h>
#include <cmath>
#include "Deal.h"

using namespace std;
using namespace cv;

class Light
{
public:
    vector<Point> contours; //轮廓的所有点
    Point2f center;         //中心点
    Point2f top;            //上顶点
    Point2f bottom;         //下顶点
    double height;          //高
    double width;           //宽
    Point2f points[4];      //记录灯条的四个顶点

    //初始化灯条
    Light(Point2f input_led[], vector<Point> contours, Point2f center);
    //增加默认构造函数
    Light(){};
    //消除椭圆拟合的误差
    void getTruePoint();
};
