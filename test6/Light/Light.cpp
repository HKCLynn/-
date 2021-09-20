/**
*@file Light.cpp
*@author 徐英杰（541223130@qq.com）
*/
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include "Light.h"
#include "Deal.h"

using namespace std;
using namespace cv;

/**
*@brief 初始化灯条
*@param input_led 灯条外界四边形的四个点信息
*/
Light::Light(Point2f input_led[], vector<Point> contours, Point2f center)
{
    //中间传输的灯条四个点信息
    for (int i = 0; i < 4; i++)
    {
        this->points[i] = input_led[i];
    }
    //排序
    sort(begin(this->points), end(this->points), [](Point2f &a, Point2f &b) -> bool
         { return a.y < b.y; });
    //灯条的上顶点
    top = (this->points[0] + this->points[1]) / 2;
    //灯条的下顶点
    bottom = (this->points[2] + this->points[3]) / 2;
    //灯条的高
    height = sqrt(pow(top.x - bottom.x, 2) + pow(top.y - bottom.y, 2));
    this->contours = contours;
    this->center = center;
}

void Light::getTruePoint()
{
    if (contourArea(this->contours) > 30)
    {
        float min_y = this->center.y;
        float max_y = this->center.y;
        Point2f new_bottom = Point(-1, -1);
        Point2f new_top = Point(-1, -1);
        float min_distance_top = MAXFLOAT;
        float min_distance_botoom = MAXFLOAT;
        for (auto point : contours)
        {
            if (point.y < min_y)
            {
                // 找出离原来上顶点最近的点作新上顶点
                float tmp_distance = get_distance(point, this->top);
                if (tmp_distance < min_distance_top)
                {
                    min_distance_top = tmp_distance;
                    new_top = point;
                }
            }
            if (point.y > max_y)
            {
                // 找出离原来下顶点最近的点作新下顶点
                float tmp_distance = get_distance(point, this->bottom);
                if (tmp_distance < min_distance_botoom)
                {
                    min_distance_botoom = tmp_distance;
                    new_bottom = point;
                }
            }
        }
        if (new_bottom != Point2f(-1, -1) || new_top != Point2f(-1, -1))
        {
            Point2f dta_top_location=new_top-this->top;
            Point2f dta_bottom_location=new_bottom-this->bottom;
            this->top = new_top;
            this->bottom = new_bottom;
            this->center = (new_top + new_bottom) / 2.f;
            this->height = get_distance(this->top, this->bottom);
            for(int i=0;i<4;i++)
            {
                if(this->points[i].y<this->center.y)
                this->points[i]=this->points[i]+dta_top_location;

                if(this->points[i].y>this->center.y)
                this->points[i]=this->points[i]+dta_bottom_location;
            }
        }
    }
}
