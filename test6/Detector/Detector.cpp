/**
 * @file Detector.cpp
 * @author 徐英杰 (541223130@qq.com)
 * @brief 返回发送数据的yaw和pitch
 * @version 1.0
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "Detector.h"



using namespace std;
using namespace cv;
/**
 * @brief 从整个程序中获得需要发给电控的目标装甲板的结构体信息
 * 
 * @param frame 处理的画面
 * @return DataStruct 发送给电控的结构体信息
 */
DataStruct Detector::get_data(Mat &frame)
{
    //定义发送给电控的结构体信息
    DataStruct send_data;
    //追踪装甲板
    ArmorTracker tracker;
    //决策信息
    Strategy strategy;

    while (1)
    {
        //传入摄像头的信息
        Mat frame;
        //处理之后的图像
        Mat mask;
        //中心点序列
        vector<Point2f> centers;
        //图像处理
        mask = imgPreprosses(frame);
        //寻找装甲板对应的灯条
        FindArmor tools(frame, mask);
        //把装甲板的中心点全部存入中心点序列
        tools.lights_pair(centers, tracker);
        //对得到的所有装甲板进行决策
        send_data=strategy.Strategy_Armors(tracker.last_armors, frame);

        //放出效果图
        imshow("windows", frame);
        imshow("windows2", mask);
        if (27 == waitKey(30))
        {
            break;
        }
    }
}
