/**
*@file Predictor.h
*@author 徐英杰（541223130@qq.com）
*/


#pragma once


#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>

using namespace std;
using namespace cv;



class Predictor
{
public:
    float flyingtime;
    //卡尔曼滤波器
    KalmanFilter kfer;
    //该帧的观测点
    Point2f next_point;
    //该帧的预测点
    Point2f this_point;
    //匹配失败的计数器
    int count=0;
    //初始化
    Predictor(KalmanFilter kfer,Point2f next_point,Point2f this_point)
    {
        this->kfer=kfer;
        this->next_point=next_point;
        this->this_point=this_point;
    }
    void update_kf(Point2f center);
    Predictor(){}
};

class Prediction
{
public:
    //创建新预测器，更新预测器，删除预测器，对灯条中心点进行预测
    void update_create_delete(vector<Point2f> &centers, vector<Predictor> &predictors, Mat &frame);
    //第一帧的观测器序列初始化
    void paint_initialization(vector<Point2f> &centers,vector<Predictor> &predictors);
    //滤波器初始化
    void kf_initialization(KalmanFilter &kf);
};




