/**
 * @file Strategy.cpp
 * @author 徐英杰 (541223130@qq.com)
 * @version 1.0
 * @date 2021-09-30
 * 
 * 
 */

#include <opencv2/opencv.hpp>
#include "Armor.h"
#include "math.h"
#include "Strategy.h"
#include "Predictor.h"

using namespace std;
using namespace cv;

void Strategy::Strategy_Armors(vector<Armor> &armors, Mat &frame)
{
    if (armors.empty())
    {
        tracking_armor.is_tracked = false;
        sendData.yaw = 0;
        sendData.pitch = 0;
    }
    else if (!armors.empty())
    {
        this->tracking_armor = *min_element(armors.begin(), armors.end(),
                                            [&](const Armor &armor_1, const Armor &armor_2)
                                            {
                                                return armor_1.PnP_data.distance < armor_2.PnP_data.distance;
                                            });
        this->tracking_armor.is_tracked = true;
    }
    else
    {
        if (tracking_armor.PnP_data.distance > 1000)
            tracking_armor.is_tracked == false;
    }

    float total_yaw = 0.f;
    float total_pitch = 0.f;

    if (tracking_armor.is_tracked)
    {
        if (tracking_armor.kf_inited)
        {
            
            KalmanFilter kftest(4,2,0);
            tracking_armor.kf_init(kftest);
            tracking_armor.update_kfer(kftest);
            total_yaw = tracking_armor.get_pre_angle().x;
            total_pitch = tracking_armor.get_pre_angle().y;
            circle(frame, tracking_armor.center, 5, Scalar(0, 0, 255), -1);
            circle(frame, tracking_armor.pre_center, 5, Scalar(0, 255, 0), -1);
            for (int k = 0; k <= 3; k++)
            {
                line(frame, tracking_armor.left_light.points[k], tracking_armor.left_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
                line(frame, tracking_armor.right_light.points[k], tracking_armor.right_light.points[(k + 1) % 4], Scalar(0, 0, 255), 2);
            }
            //画出对角线
            line(frame, tracking_armor.left_light.top, tracking_armor.right_light.bottom, Scalar(0, 0, 255), 2);
            line(frame, tracking_armor.right_light.top, tracking_armor.left_light.bottom, Scalar(0, 0, 255), 2);
        }
        else
        {
            KalmanFilter kftest(4,2,0);
            tracking_armor.kf_init(kftest);
            tracking_armor.update_kfer(kftest);
            total_yaw = tracking_armor.get_pre_angle().x;
            total_pitch = tracking_armor.get_pre_angle().y;
            circle(frame, tracking_armor.center, 5, Scalar(0, 0, 255), -1);
            circle(frame, tracking_armor.pre_center, 5, Scalar(0, 255, 0), -1);

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
}