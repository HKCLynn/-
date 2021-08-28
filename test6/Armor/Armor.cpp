/**
*@file Armor.cpp
*@author 徐英杰（541223130@qq.com）
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include "Light.h"
#include "Deal.h"
#include "Armor.h"

using namespace std;
using namespace cv;

/**
*@brief 初始化
*@param frame 传入未处理的图
*@param mask 传入二值化等图像处理后的图
*/

FindArmor::FindArmor(Mat frame, Mat mask)
{
    this->frame = frame;
    this->mask = mask;
}

/**
*@brief 灯条寻找并配对
*@param centers 配对后将装甲板中心点存入的序列
*/

void FindArmor::lights_pair(vector<Point2f> &centers)
{
    //在二值化等图像处理后的图片中寻找轮廓
    findContours(mask, contours_all, hierarchy_all, RETR_EXTERNAL, CHAIN_APPROX_NONE, Point());
    //存放每个灯条四个点坐标的容器
    vector<RotatedRect> light_info;
    //存放灯条的容器
    vector<Light> lights;
    for (int i = 0; i < contours_all.size(); i++)
    {
        //椭圆拟合
        RotatedRect r = fitEllipse(contours_all[i]);
        //对于每一个轮廓进行筛选得到灯条
        if (r.size.height / r.size.width < 7 &&
            r.size.height / r.size.width > 1.5 &&
            contourArea(contours_all[i]) > 10 &&
            contourArea(contours_all[i]) / r.size.area() > 0.6 &&
            r.center.x - r.size.width > 0 &&
            r.center.y - r.size.height > 0 &&
            (r.angle <= 45 ||
             r.angle >= 135))
        {
            //灯条的四个点信息
            Point2f point[4];
            r.points(point);
            //插入容器中
            light_info.push_back(r);
            //创建灯条对象
            Light input_light(point);
            //将筛选出的灯条插入容器中
            lights.push_back(input_light);
        }
    }
    //装甲板容器
    vector<Armor> armors;
    //对所有筛选出来的灯条进行遍历
    for (int i = 0; i < lights.size(); i++)
    {
        for (int j = i + 1; j < lights.size(); j++)
        {
            //未筛选的“装甲板”
            Armor input_armor;
            //水平宽度
            input_armor.middleWidth = get_distance(light_info[i].center, light_info[j].center);
            //平均高度
            input_armor.middleHeight = (lights[i].height + lights[j].height) / 2;
            //任意两个灯条y坐标的偏差
            double distanceY = lights[i].top.y - lights[j].top.y;
            //两条对角线的长度
            double oppositeOne = get_distance(lights[i].top, lights[j].bottom);
            double oppositeTwo = get_distance(lights[j].top, lights[i].bottom);
            //对灯条进行配对
            if (abs(light_info[i].center.y - light_info[j].center.y) / ((lights[i].height + lights[j].height) / 2) < 0.7 &&
                abs(light_info[i].center.y - light_info[j].center.y) / ((lights[i].height + lights[j].height) / 2) > 0.0001 &&
                abs(light_info[i].center.x - light_info[j].center.x) / ((lights[i].height + lights[j].height) / 2) > 0.5 &&
                abs(light_info[i].angle - light_info[j].angle) < 5 &&
                input_armor.middleWidth / input_armor.middleHeight < 3.3 &&
                input_armor.middleWidth / input_armor.middleHeight > 1.1 &&
                abs(lights[i].height - lights[j].height) < 20 &&
                abs(oppositeOne - oppositeTwo) / input_armor.middleWidth < 0.115 &&
                i != j)
            {
                //配对成功后分别储存这两个灯条的四个点信息
                Point2f pointOne[4], pointTwo[4];
                light_info[i].points(pointOne);
                light_info[j].points(pointTwo);


                //得到装甲板中心点坐标
                input_armor.center = (light_info[i].center + light_info[j].center) / 2;
                //画出中心点
                circle(frame, input_armor.center, 5, Scalar(0, 0, 255), -1);
                //插入装甲板的容器里
                armors.push_back(input_armor);
                //将中心点放入之前输入的中心点容器中
                centers.push_back(input_armor.center);
                //画出配对成功的灯条的外接矩形
                for (int j = 0; j <= 3; j++)
                {
                    line(frame, pointOne[j], pointOne[(j + 1) % 4], Scalar(0, 0, 255), 2);
                    line(frame, pointTwo[j], pointTwo[(j + 1) % 4], Scalar(0, 0, 255), 2);
                }
                //画出对角线
                line(frame, lights[i].top, lights[j].bottom, Scalar(0, 0, 255), 2);
                line(frame, lights[j].top, lights[i].bottom, Scalar(0, 0, 255), 2);
            }
        }
    }
}
