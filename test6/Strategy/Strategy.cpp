/**
 * @file Strategy.cpp
 * @author 徐英杰 (541223130@qq.com)
 * @version 1.0
 * @date 2021-09-30
 * 
 * 
 */

#include "Strategy.h"

using namespace std;
using namespace cv;

/**
 * @brief 对得到的所有装甲板进行决策
 * 
 * @param armors 
 * @param frame 
 * @return DataStruct 获得需要发送给电控的信息
 */
DataStruct Strategy::Strategy_Armors(vector<Armor> &armors, Mat &frame)
{
    //如果画面中没有装甲板
    if (armors.empty())
    {
        //巡逻模式
        this->patrol_mode();
        //未跟踪
        tracking_armor.is_tracked = false;
    }
    //如果画面中有装甲板但是没有跟踪装甲板
    else if (tracking_armor.is_tracked == false && !armors.empty())
    {
        //获得最近的装甲板作为跟踪目标
        this->tracking_armor = *min_element(armors.begin(), armors.end(),
                                            [&](const Armor &armor_1, const Armor &armor_2)
                                            {
                                                return armor_1.PnP_data.distance < armor_2.PnP_data.distance;
                                            });
        //已跟踪
        this->tracking_armor.is_tracked = true;
    }
    //如果已经有跟踪的目标
    else
    {
        //配对成功标志位
        int is_match = 0;
        //对当前所有的装甲板和之前跟踪的装甲板进行配对
        for (int i = 0; i < armors.size(); i++)
        {
            //如果距离够近配对成功
            if (get_distance(armors[i].center, tracking_armor.center) < 100000)
            {
                //保留更新后的卡尔曼滤波
                armors[i].kf = tracking_armor.kf;
                //将跟踪的装甲板进行更新
                tracking_armor = armors[i];
                //已跟踪
                tracking_armor.is_tracked = true;
                //已跟踪
                is_match = 1;
            }
        }
        //如果未配对
        if (!is_match)
        {
            //则没有跟踪到装甲板
            tracking_armor.is_tracked == false;
        }
        //如果距离过大
        if (tracking_armor.PnP_data.distance > 100)
            //取消跟踪装甲板
            tracking_armor.is_tracked == false;
    }

    float total_yaw = 0.f;
    float total_pitch = 0.f;
    //如果跟踪到装甲板
    if (tracking_armor.is_tracked)
    {
        //如果装甲板的卡尔曼滤波已经初始化过
        if (tracking_armor.kf_inited)
        {
            //进行更新
            tracking_armor.update_kfer();
            //获得预测的yaw
            total_yaw = tracking_armor.get_pre_angle().x;
            //获得预测的pitch
            total_pitch = tracking_armor.get_pre_angle().y;
            //追踪模式
            this->track_mode(total_yaw, total_pitch);
            //画出追踪的装甲板信息
            circle(frame, tracking_armor.pre_center, 5, Scalar(0, 255, 0), -1);
            circle(frame, tracking_armor.center, 5, Scalar(0, 0, 255), -1);
            //画出配对成功的灯条的外接矩形
            for (int k = 0; k <= 3; k++)
            {
                line(frame, tracking_armor.left_light.points[k], tracking_armor.left_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
                line(frame, tracking_armor.right_light.points[k], tracking_armor.right_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
            }
            //画出对角线
            line(frame, tracking_armor.left_light.top, tracking_armor.right_light.bottom, Scalar(0, 0, 255), 2);
            line(frame, tracking_armor.right_light.top, tracking_armor.left_light.bottom, Scalar(0, 0, 255), 2);
        }
    }
    //返回发给电控的信息
    return this->send_data;
}

/**
 * @brief 巡逻模式
 */
void Strategy::patrol_mode()
{
    this->send_data.Flag = MergeCommand(CommandChassis::Patrol, CommandHolder::Spin, CommandModular::No_detected);
    this->send_data.shoot_mode = MergeCommand(CommandChassis::Randon, CommandHolder::No_shoot, CommandModular::No_detected);
    this->send_data.yaw = 0;
    this->send_data.pitch = 0;
}

/**
 * @brief 追踪模式
 */
void Strategy::track_mode(const double &X, const double &Y)
{
    this->send_data.Flag = MergeCommand(CommandChassis::Randon, CommandHolder::Track, CommandModular::Detected);
    this->send_data.shoot_mode = MergeCommand(CommandChassis::Randon, CommandHolder::No_shoot, CommandModular::Detected);
    this->send_data.yaw = X;
    this->send_data.pitch = Y;
}

/**
 * @brief 普通射击模式
 * 
 * @param Yaw x方向角度
 * @param Pitch y方向角度
 */
void Strategy::shoot_mode(const double &X, const double &Y)
{
    this->send_data.Flag = MergeCommand(CommandChassis::Randon, CommandHolder::Track, CommandModular::Detected);
    this->send_data.shoot_mode = MergeCommand(CommandChassis::Randon, CommandHolder::Single_Shoot, CommandModular::Detected);
    this->send_data.yaw = X;
    this->send_data.pitch = Y;
}