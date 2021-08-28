/**
*@file test7.cpp
*@author 徐英杰（541223130@qq.com）
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include "DealWith/DealWith.h"
#include "Prediction/Prediction.h"
#include "Identifier/Identifier.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    //实例化相机
    VideoCapture video("/home/lynn/text1.mp4");
    //新建一个卡尔曼滤波器
    KalmanFilter KF(2, 1, 0);
    //预测对象
    Prediction pre(KF);
    Point2f now_center(0, 0);
    //是否是第一帧的标志
    int start = 0;
    while (1)
    {
        //摄像头每一帧的图片
        Mat frame;
        video >> frame; //传入摄像头每一帧图片
        //图像处理后的图像
        Mat dstImage;
        dstImage = imgPreprosses(frame);
        //新建识别器
        Identifier ide(dstImage, frame);
        //对需要打击的板和能量机关中心进行识别
        ide.identify();
        //如果不是第一帧且前后两帧的点距离太远，则重新对识别器的卡尔曼滤波进行初始化，重新寻找目标
        if (start == 1 && get_distance(now_center, ide.rec_rec.center) > 30)
        {
            //卡尔曼滤波初始化
            pre.kf_initialization(pre.kf); 
            //对中心点进行预测
            pre.kf_prediction(ide.rec_rec.center, ide.cen_rec.center, frame);
            //对于预测出来的点进行迭代
            ide.rec_rec.center = pre.aft_point;
            //画出预测到的打击版的位置的矩形
            Point2f write_points[4];
            ide.rec_rec.points(write_points);
            write_rec(write_points, frame);
            //更新当前帧的点
            now_center = ide.rec_rec.center;
            //不执行下面那个if语句，跳到下一次循环，一个循环只需要预测一次
            continue;
        }
        if (ide.cen_rec.center.x > 10 && ide.rec_rec.center.x > 10)
        {
            //对得到的能量机关中心点和打击板的中心点进行预测
            pre.kf_prediction(ide.rec_rec.center,ide.cen_rec.center,frame);
            //对于预测出来的点进行迭代
            ide.rec_rec.center=pre.aft_point;
            //画出预测到的打击板的位置的矩形
            Point2f write_points[4];
            ide.rec_rec.points(write_points);
            write_rec(write_points,frame);
            //更新当前帧的点
            now_center = ide.rec_rec.center;
            //在此之后不是第一帧
            start = 1;
        }

        imshow("windows", dstImage); //放出二值化，闭运算之后的图像
        imshow("frame", frame);      //放出原图像
        waitKey(30);
    }
}
