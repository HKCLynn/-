/**
 * @file Detector.h
 * @author 徐英杰 (541223130@qq.com)
 * @brief 返回发送数据的yaw和pitch
 * @version 1.0
 * @date 2021-10-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once
#include "getdata.h"
#include "Strategy.h"
#include "GyroScope.h"

//检测器
class Detector
{
public:
    DataStruct send_data;            //发送给电控的数据
    DataStruct get_data(Mat &frame, ArmorTracker &tracker, Strategy &strategy ,float &,GyroData); //获得发送给电控的数据
};
