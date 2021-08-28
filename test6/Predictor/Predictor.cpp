/**
*@file Predictor.cpp
*@author 徐英杰（541223130@qq.com）
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <math.h>
#include "Predictor/Predictor.h"
#include "Deal/Deal.h"

using namespace std;
using namespace cv;


/**
*@brief 创建新观测器，更新观测器，删除观测器，对灯条中心点进行预测
*@param centers 该帧中心点的测量点序列
*@param predictors 上一帧的预测器序列
*/
void Prediction::update_create_delete(vector<Point2f> &centers, vector<Predictor> &predictors, Mat &frame)
{
    //观测量
    Mat measurement = Mat::zeros(2, 1, CV_32F);
    //先遍历上一帧的预测器，再遍历当前的测量点
    for (int j = 0; j < predictors.size(); j++)
    {
        //预测器与测量点匹配的标志
        int is_match = 0;
        for (int i = 0; i < centers.size(); i++)
        {
            //如果上一帧预测器中的测量点与当前的测量点距离小于20，则匹配成功
            if (get_distance(centers[i], predictors[j].this_point) < 20)
                is_match = 1;
        }
        if (is_match == 0)
            predictors[j].count++;                    //若和该帧的任何点匹配不成功，则计数器加1
        if (predictors[j].count >= 5)                 //若计数器中的数大于5，说明至少连续5帧预测器中的点都没有和任何测量点匹配
            predictors.erase(predictors.begin() + j); //删除该预测器
    }
    //该帧的测量点与上帧的测量点进行匹配
    for (int i = 0; i < centers.size(); i++)
    {
        //该帧的测量点与上一帧是否有匹配的测量点的判断
        int is_match = 0;
        for (int j = 0; j < predictors.size(); j++)
        {
            //如果两点坐标距离不超过20，匹配成功
            if (get_distance(centers[i], predictors[j].this_point) < 20)
            {
                //将这一帧匹配成功的点更新
                predictors[j].this_point = centers[i];

                measurement.at<float>(0) = predictors[j].this_point.x;
                measurement.at<float>(1) = predictors[j].this_point.y;

                //更新进滤波器中
                predictors[j].kfer.correct(measurement);
                //将这一帧的点进行预测，并保存预测点
                Mat prediction = predictors[j].kfer.predict();
                predictors[j].next_point = Point(prediction.at<float>(0), prediction.at<float>(1));

                //画出预测点
                circle(frame, predictors[j].next_point, 8, Scalar(0, 255, 0), -1);
                //匹配成功的标志
                is_match = 1;
                //计数器清零
                predictors[j].count = 0;
            }
        }
        //若该帧的测量点与预测器中上一帧任何测量点都没有匹配
        if (is_match == 0)
        {
            //该点创建一个新的预测器对象
            //新卡尔曼滤波对象
            KalmanFilter new_kf(4, 2, 0);
            //新观测量
            Mat new_measurement = Mat::zeros(2, 1, CV_32F);
            //初始化
            kf_initialization(new_kf);
            //更新
            new_measurement.at<float>(0) = centers[i].x;
            new_measurement.at<float>(1) = centers[i].y;
            new_kf.correct(new_measurement);
            //预测
            Mat prediction = new_kf.predict();
            //该测量点的预测点
            Point2f pre_point = Point(prediction.at<float>(0), prediction.at<float>(1));
            //创建新的预测器对象，存入的是该帧的测量点和该帧预测的下一帧的点，卡尔曼滤波对象
            Predictor new_point(new_kf, pre_point, centers[i]);
            //插入至预测器序列中
            predictors.push_back(new_point);
        }
    }
}

/**
*@brief 第一帧的测量点初始化
*@param centers 该帧中心点的测量点序列
*@param predictors 新的预测器序列
*/
void Prediction::paint_initialization(vector<Point2f> &centers, vector<Predictor> &predictors)
{
    //对每一个中心点遍历
    for (int i = 0; i < centers.size(); i++)
    {
        //得到卡尔曼滤波器
        KalmanFilter kf(4, 2, 0);
        //观测量
        Mat measurement = Mat::zeros(2, 1, CV_32F);
        measurement.at<float>(0) = centers[i].x;
        measurement.at<float>(1) = centers[i].y;
        //初始化
        kf_initialization(kf);
        //更新
        kf.correct(measurement);
        //预测
        Mat prediction = kf.predict();
        //第一帧的预测点
        Point2f nextPoint = Point2f(measurement.at<float>(0), measurement.at<float>(1));
        //分别保存在第一帧刚开始的新的预测器中
        Predictor startPoint(kf, nextPoint, centers[i]);
        //插入至预测器序列
        predictors.push_back(startPoint);
    }
}
/**
*@brief 卡尔曼滤波器初始化
*@param kf 未初始化的卡尔曼滤波器
*/
void Prediction::kf_initialization(KalmanFilter &kf)
{
    RNG rng;
    kf.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1); //转移矩阵A
    setIdentity(kf.measurementMatrix);                                                           //测量矩阵H
    setIdentity(kf.processNoiseCov, Scalar::all(1e-5));                                          //系统噪声方差矩阵Q
    setIdentity(kf.measurementNoiseCov, Scalar::all(1e-7));                                      //测量噪声方差矩阵R
    setIdentity(kf.errorCovPost, Scalar::all(1));                                                //后验错误估计协方差矩阵P
    rng.fill(kf.statePost, RNG::UNIFORM, 0, 0);                                                  //初始状态值x(0)
}
