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

void FindArmor::lights_pair(vector<Point2f> &centers, ArmorTracker &Tracker)
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
        r.angle = (r.angle > 90 ? r.angle - 180 : r.angle);
        //对于每一个轮廓进行筛选得到灯条
        if (r.size.height / r.size.width < 7 &&
                r.size.height / r.size.width > 1.5 &&
                contourArea(contours_all[i]) > 10 &&
                contourArea(contours_all[i]) / r.size.area() > 0.6 &&
                r.center.x - r.size.width > 0 &&
                r.center.y - r.size.height > 0 &&
                contourArea(contours_all[i]) > 10 &&
                r.angle <= 45 ||
            r.angle >= -45)
        {
            //灯条的四个点信息
            Point2f point[4];
            r.points(point);
            //插入容器中
            light_info.push_back(r);
            //创建灯条对象
            Light input_light(point, contours_all[i], r.center);
            //将筛选出的灯条插入容器中
            lights.push_back(input_light);
            drawContours(frame, contours_all, i, (255, 255, 255), 10);
        }
    }
    //装甲板容器
    vector<Armor> armors;
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
            if (abs(light_info[i].center.y - light_info[j].center.y) / ((lights[i].height + lights[j].height) / 2) > 0.7)
                continue;
            if (abs(light_info[i].center.x - light_info[j].center.x) / ((lights[i].height + lights[j].height) / 2) < 0.5)
                continue;
            if (abs(light_info[i].angle - light_info[j].angle) > 5.5)
                continue;
            if (input_armor.middleWidth / input_armor.middleHeight > 3.3 ||
                input_armor.middleWidth / input_armor.middleHeight < 1.5)
                continue;

            if (abs(lights[i].height - lights[j].height) > 20)
                continue;
            if (abs(oppositeOne - oppositeTwo) / input_armor.middleWidth > 0.115)
                continue;

            //配对成功后分别储存这两个灯条的四个点信息
            light_info[i].points(lights[i].points);
            light_info[j].points(lights[j].points);
            lights[i].getTruePoint();
            lights[j].getTruePoint();
            input_armor.left_light = lights[i];
            input_armor.right_light = lights[j];
            //得到装甲板中心点坐标
            input_armor.center = (light_info[i].center + light_info[j].center) / 2;
            //画出中心点
            circle(frame, input_armor.center, 5, Scalar(0, 0, 255), -1);
            armors.push_back(input_armor);
            //插入装甲板的容器里
            if (Tracker.start == 0)
            {
                Tracker.last_armors.push_back(input_armor);
                //将中心点放入之前输入的中心点容器中
                centers.push_back(input_armor.center);
            }
            else if (Tracker.start == 1)
            {
                Tracker.now_armors.push_back(input_armor);
                centers.push_back(input_armor.center);
            }
            else
                centers.push_back(input_armor.center);
        }
    }
    if (Tracker.start > 1)
    {
        Tracker.now_armors.clear();
        for (auto i = 0; i < armors.size(); i++)
        {
            Tracker.now_armors.push_back(armors[i]);
        }
        Tracker.update_armors();
        centers.clear();
        for (int i = 0; i < Tracker.last_armors.size(); i++)
        {
            centers.push_back(Tracker.last_armors[i].center);
        }
    }
    Tracker.start++;
}

void FindArmor::writing(vector<Armor> armors)
{
    for (int i = 0; i < armors.size(); i++)
    {
        //画出配对成功的灯条的外接矩形
        for (int k = 0; k <= 3; k++)
        {
            line(this->frame, armors[i].left_light.points[k], armors[i].left_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
            line(this->frame, armors[i].right_light.points[k], armors[i].right_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
        }
        //画出对角线
        line(this->frame, armors[i].left_light.top, armors[i].right_light.bottom, Scalar(0, 0, 255), 2);
        line(this->frame, armors[i].right_light.top, armors[i].left_light.bottom, Scalar(0, 0, 255), 2);
    }
}

void ArmorTracker::update_armors()
{
    for (int i = 0; i < this->last_armors.size(); i++)
    {
        bool is_match = false;
        for (int j = 0; j < this->now_armors.size(); j++)
        {
            if (get_distance(last_armors[i].center, now_armors[j].center) < 100)
            {
                is_match = true;
                last_armors[i] = now_armors[j];
                last_armors[i].dis_count = 0;
            }
        }
        if (is_match == false)
        {
            last_armors[i].dis_count++;
            now_armors.push_back(last_armors[i]);
        }
    }

    for (int j = 0; j < this->now_armors.size(); j++)
    {
        int is_match = false;
        for (int i = 0; i < this->last_armors.size(); i++)
        {
            if (get_distance(last_armors[i].center, now_armors[j].center) < 100)
            {
                is_match = true;
            }
        }
        if (is_match == false)
        {
            last_armors.push_back(now_armors[j]);
        }
    }
    for (int i = 0; i < last_armors.size(); i++)
    {
        if (last_armors[i].dis_count > 2)
        {
            last_armors.erase(last_armors.begin() + i);
        }
    }
}