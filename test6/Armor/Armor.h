/**
*@file Armor.h
*@author 徐英杰（541223130@qq.com）
*/

#pragma once
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Core>
#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include "Parameter.h"
#include "Deal.h"
#include "Light.h"
#include "GyroScope.h"

using namespace std;
using namespace cv;

struct ResultPnP
{
public:
    double yaw;
    double pitch;
    double roll;
    Mat tran_vec;
    Mat rotat_vec;
    float distance;
};

//pnp信息
class Armor
{
public:
    double middleWidth;         //水平平均宽度
    double middleHeight;        //平均高度
    Point2f center;             //中心点
    Point2f pre_center;         //预测的中心点
    Light left_light;           //左灯条
    Light right_light;          //右灯条
    int dis_count = 0;          //消失帧数
    bool is_tracked = false;    //是否被跟踪
    ResultPnP PnP_data;         //PnP的数据
    KalmanFilter kf_dis;        //对距离的卡尔曼滤波
    KalmanFilter kf;            //对画面位置的卡尔曼滤波
    bool kf_inited = false;     //位置的卡尔曼滤波的初始化标志位
    bool kf_inited_dis = false; //距离的卡尔曼滤波的初始化标志位
    int64 appear_time;          // 追踪匹配信息 -- 每帧装甲板出现的时间
    Point2f gyro_location;      // 陀螺仪位置
    Point2f relative_angles;    // 追踪匹配信息 -- 图像坐标系中的角度

    void get_pnp();          //获得pnp信息
};

class ArmorTracker
{
public:
    int start = 0;             //是否为第一帧
    vector<Armor> now_armors;  //当前帧的所有装甲板
    vector<Armor> last_armors; //上一帧存在的装甲板

    void update_armors(); //对装甲板的更新
};

class FindArmor
{
public:
    GyroData gyrodata;
    Mat frame;                          //未处理的图像
    Mat mask;                           //处理后的图像
    vector<vector<Point>> contours_all; //所有轮廓
    vector<Vec4i> hierarchy_all;        //轮廓信息

    //灯条寻找并配对
    void lights_pair(vector<Point2f> &centers, ArmorTracker &Tracker, float &);
    //初始化
    FindArmor(Mat frame, Mat mask, GyroData gyrodata);
};
