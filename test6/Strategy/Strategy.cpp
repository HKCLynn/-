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
    if (this->is_tracked == false && armors.empty())
    {
        //巡逻模式
        this->patrol_mode();
        //未跟踪
        is_tracked = false;
    }
    //如果画面中有装甲板但是没有跟踪装甲板
    else if (!this->is_tracked && !armors.empty())
    {
        tracker.clear();
        //获得最近的装甲板作为跟踪目标
        this->new_armor = *min_element(armors.begin(), armors.end(),
                                       [&](const Armor &armor_1, const Armor &armor_2)
                                       {
                                           return armor_1.PnP_data.distance < armor_2.PnP_data.distance;
                                       });
        this->kf_init(new_armor.center);
        //已跟踪
        this->is_tracked = true;
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
            if (tracker.size() != 0 && get_distance(armors[i].center, tracker[0].center) < 1000000000000000)
            {
                if (tracker.size() > 8)
                {
                    tracker.push_front(armors[i]);
                    tracker.pop_back();
                }
                else
                {
                    tracker.push_front(armors[i]);
                }
                is_match = 1;
            }
        }
        //如果未配对
        if (!is_match)
        {
            //则没有跟踪到装甲板
            this->is_tracked = false;
            tracker.clear();
        }
        //如果距离过大
        if (tracker[0].PnP_data.distance > 100000 || tracker[0].dis_count >= 8)
        {
            this->is_tracked = false;
            tracker.clear();
        }
    }
    float total_yaw = 0.f;
    float total_pitch = 0.f;
    //如果跟踪到装甲板
    if (this->is_tracked)
    {
        //画出追踪的装甲板信息
        circle(frame, tracker[0].center, 5, Scalar(0, 0, 255), -1);
        //画出配对成功的灯条的外接矩形
        for (int k = 0; k <= 3; k++)
        {
            line(frame, tracker[0].left_light.points[k], tracker[0].left_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
            line(frame, tracker[0].right_light.points[k], tracker[0].right_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
        }
        //画出对角线
        line(frame, tracker[0].left_light.top, tracker[0].right_light.bottom, Scalar(0, 0, 255), 2);
        line(frame, tracker[0].right_light.top, tracker[0].left_light.bottom, Scalar(0, 0, 255), 2);

        kf_pre(total_yaw, total_pitch, frame);

        this->track_mode(total_yaw, total_pitch);
    }
    //返回发给电控的信息
    return this->send_data;
}

void Strategy::kf_pre(float &total_yaw, float &total_pitch, Mat frame)
{
    Mat measurement = Mat::zeros(4, 1, CV_32F);
    Mat prediction;
    measurement.at<float>(0) = (float)tracker[0].center.x;
    measurement.at<float>(1) = (float)tracker[0].center.y;
    measurement.at<float>(3)=this->speed.x;
    measurement.at<float>(4)=this->speed.y;
    Point2f center = tracker[0].center;
    if (tracker.size() < 6)
    {
        prediction = filter.predict();
        this->filter.correct(measurement);
    }
    else
    {
        prediction = filter.predict();
        this->filter.correct(measurement);
        Point2f pre_point = Point2f(prediction.at<float>(0), prediction.at<float>(1));
        total_yaw = get_angle(pre_point).x;
        total_pitch = get_angle(pre_point).y;
        circle(frame, pre_point, 5, Scalar(0, 255, 0), -1);
    }
}

Point2f Strategy::get_angle(Point2f pre_center)
{
    //相机内参矩阵
    Mat camMatrix = (Mat_<double>(3, 3) << 1.6041191539594568e+03, 0., 6.3983687194220943e+02, 0.,
                     1.6047833493341714e+03, 5.1222951297937527e+02, 0., 0., 1.);

    //相机畸变矩阵
    Mat distCoeff = (Mat_<double>(5, 1) << -6.3687295852461456e-01, -1.9748008790347320e+00,
                     3.0970703651800782e-02, 2.1944646842516919e-03, 0.);
    return calculateRelativeAngle(camMatrix, distCoeff, pre_center);
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

void Strategy::kf_init(Point2f point)
{
    this->filter.init(4, 2, 0);
    filter.transitionMatrix = (Mat_<float>(4, 4) << 1, 0, 5, 0,
                               0, 1, 0, 5,
                               0, 0, 1, 0,
                               0, 0, 0, 1);                    //转移矩阵A
    setIdentity(filter.measurementMatrix);                     //测量矩阵H
    setIdentity(filter.processNoiseCov, Scalar::all(1e-5));    //系统噪声方差矩阵Q
    setIdentity(filter.measurementNoiseCov, Scalar::all(1e1)); //测量噪声方差矩阵R
    setIdentity(filter.errorCovPost, Scalar::all(1));          //后验错误估计协方差矩阵P
    filter.statePost = (Mat_<float>(4, 1) << point.x,
                        point.y,
                        0,
                        0);
}

void Strategy::get_speed()
{
    float total_time = 0;
    Point2f gyro_speed;
    Point2f target_speed;
    if (tracker.size() >= 4)
    {
        total_time = (this->tracker[0].appear_time +
                      tracker[1].appear_time -
                      tracker[2].appear_time -
                      tracker[3].appear_time) /
                     getTickFrequency() / 2.f;

        gyro_speed = (this->tracker[0].gyro_location +
                              tracker[1].gyro_location -
                              tracker[2].gyro_location -
                              tracker[3].gyro_location) /
                             (2.f * total_time);

        target_speed = (this->tracker[0].relative_angles +
                                tracker[1].relative_angles -
                                tracker[2].relative_angles -
                                tracker[3].relative_angles) /
                               (2.f * total_time);
    }
    else
    {
        total_time = (this->tracker[0].appear_time + -tracker.back().appear_time) /
                     getTickFrequency() / 2.f;

        gyro_speed = (this->tracker[0].gyro_location - tracker.back().gyro_location) /
                             (2.f * total_time);

        target_speed = (this->tracker[0].relative_angles - tracker.back().relative_angles) /
                               (2.f * total_time);
    }
    this->speed = target_speed - gyro_speed;
}