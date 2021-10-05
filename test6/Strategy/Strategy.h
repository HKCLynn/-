/**
 * @file Strategy.h
 * @author 徐英杰 (541223130@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-10-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <cmath>
#include "Armor.h"
#include "getdata.h"

using namespace std;
using namespace cv;

class Strategy
{
public:
    //对得到的所有装甲板进行决策，返回决策出跟踪的装甲板的结构体信息
    DataStruct Strategy_Armors(vector<Armor> &tracking_armor, Mat &frame);
    Armor tracking_armor;                                    //跟踪的装甲板
    DataStruct send_data;                                    //跟踪的装甲板的结构体信息
    void patrol_mode();                                      // 巡逻模式
    void track_mode(const double &Yaw, const double &Pitch); // 追踪模式
    void shoot_mode(const double &Yaw, const double &Pitch); // 射击模式
};


