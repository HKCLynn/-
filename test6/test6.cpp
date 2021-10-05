/**
 * @file test6.cpp
 * @author 徐英杰 (541223130@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-10-05
 * 
 * 
 */

#include <iostream>
#include <stdio.h>
#include"Visioncontrol.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    ArmorTracker tracker;
    Strategy strategy;
    //实例化相机
    VideoCapture video("/home/lynn/桌面/-/test6/video/test5.avi");
    //是否是第一帧的标志
    int start = 0;
    while (1)
    {
        //传入摄像头的信息
        Mat frame;
        video >> frame;
        Mat mask;
        //中心点序列
        vector<Point2f> centers;
        //图像处理
        mask = imgPreprosses(frame);
        //寻找装甲板对应的灯条
        FindArmor tools(frame, mask);
        //把装甲板的中心点全部存入中心点序列
        tools.lights_pair(centers,tracker);
        strategy.Strategy_Armors(tracker.last_armors,frame);


        //放出效果图
        imshow("windows", frame);
        imshow("windows2",mask);
        if (27 == waitKey(30))
        {
            break;
        }
    }
}