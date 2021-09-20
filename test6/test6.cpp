

#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>

#include "Light/Light.h"
#include "Predictor/Predictor.h"
#include "Deal/Deal.h"
#include "Armor/Armor.h"

using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    //预测器序列
    vector<Predictor> predictors;
    //实例化相机
    VideoCapture video("/home/lynn/视频/test5.avi");
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
        tools.lights_pair(centers);
        //预测方法
        Prediction prediction;

        //如果不是第一帧
        if (start != 0)
        { //更新删除添加，预测操作
            prediction.update_create_delete(centers, predictors, frame);
        }
        //如果是第一帧
        if (start == 0)
        {
            //第一帧预测器的初始化
            prediction.paint_initialization(centers, predictors);
            //在此之后不是第一帧
            start = 1;
        }
        //放出效果图
        imshow("windows", frame);
        imshow("windows2",mask);
        if (27 == waitKey(30))
        {
            break;
        }
    }
}