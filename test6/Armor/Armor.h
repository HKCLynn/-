/**
*@file Armor.h
*@author 徐英杰（541223130@qq.com）
*/

#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include "Light/Light.h"
#include"Deal.h"
using namespace std;
using namespace cv;

class Armor
{
public:
    //水平平均宽度
    double middleWidth;
    //平均高度
    double middleHeight;
    //中心点
    Point2f center;
    Light left_light;
    Light right_light;
    int dis_count = 0;
    bool is_tracked=false;
    ResultPnP PnP_data;
    KalmanFilter kf_dis;
    KalmanFilter kf;
    void get_pnp();
};

class ArmorTracker
{
public:
    int start=0;
    vector<Armor> now_armors;
    vector<Armor> last_armors;
    void update_armors();
};

class FindArmor
{
public:

    //未处理的图像
    Mat frame;
    //处理后的图像
    Mat mask;
    //所有轮廓
    vector<vector<Point>> contours_all;
    //轮廓信息
    vector<Vec4i> hierarchy_all;
    //灯条寻找并配对
    void lights_pair(vector<Point2f> &centers, ArmorTracker &Tracker);
    //初始化
    FindArmor(Mat frame, Mat mask);
    void writing(vector<Armor>armors);
};
