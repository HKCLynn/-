#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "Prediction.h"
#include "DealWith.h"


using namespace std;
using namespace cv;

Point calcPoint(Point2f center, double r, double angle)
{
    return center + Point2f((float)cos(angle), (float)sin(angle)) * (float)r;
}

Prediction::Prediction(KalmanFilter &kf)
{
    this->kf = kf;

    kf_initialization(this->kf);
}

void Prediction::kf_initialization(KalmanFilter &kf)
{
    kf.transitionMatrix = (Mat_<float>(2, 2) << 1, 1, 0, 1); //转移矩阵A[1,1;0,1]

    //将下面几个矩阵设置为对角阵
    setIdentity(kf.measurementMatrix);                       //测量矩阵H
    setIdentity(kf.processNoiseCov, Scalar::all(1e-5));      //系统噪声方差矩阵Q
    setIdentity(kf.measurementNoiseCov, Scalar::all(1e-11)); //测量噪声方差矩阵R
    setIdentity(kf.errorCovPost, Scalar::all(1));            //后验错误估计协方差矩阵P

    randn(kf.statePost, Scalar::all(0), Scalar::all(0));
}

void Prediction::kf_prediction(Point2f &rec_center, Point2f &cen_center, Mat &frame)
{
    Point2f one = cen_center;
    Point2f two = rec_center;
    center = cen_center;
    r = get_distance(one, two);
    angle = atan2(two.y - one.y, two.x - one.x);
    measurement.at<float>(0) = angle;
    kf.correct(measurement);
    Mat prediction = kf.predict();
    angle = prediction.at<float>(0);
    aft_point = calcPoint(center, r, angle);
    circle(frame, aft_point, 2, Scalar(0, 255, 0), -1);

}

