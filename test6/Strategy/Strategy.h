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
#include "GyroScope.h"
#include "getdata.h"

using namespace std;
using namespace cv;

class Strategy
{
public:
    //对得到的所有装甲板进行决策，返回决策出跟踪的装甲板的结构体信息
    DataStruct Strategy_Armors(vector<Armor> &armor, Mat &frame);
    deque<Armor> tracker; // 跟踪的装甲板
    Point2f speed;        //追踪的装甲板的绝对速度
    Armor new_armor;
    KalmanFilter filter;
    DataStruct send_data;
    Point2f get_angle(Point2f pre_center);
    void kf_init(Point2f);
    void patrol_mode();                                      // 巡逻模式
    void track_mode(const double &Yaw, const double &Pitch); // 追踪模式
    void shoot_mode(const double &Yaw, const double &Pitch); // 射击模式
    void kf_pre(float &, float &, Mat);
    void get_speed();
    // void test()
    // {
    //     if ()
    //     {
    //         tracker.clear();
    //         filter.init(4, 2, 0);
    //     }
    //     tracker.push_back(xxx);
    //     if (tracker.size() > 8)
    //     {
    //         tracker.pop_front();
    //     }
    //     if (tracker.size() > 4)
    //     {
    //         // A
    //         // correct
    //         // predict
    //     }
    //     else
    //     {
    //     }
    // }
    bool is_tracked = false;
};
