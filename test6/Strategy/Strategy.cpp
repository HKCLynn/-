

#include <opencv2/opencv.hpp>
#include "Armor.h"

#include "Strategy.h"
#include "Predictor.h"

using namespace std;
using namespace cv;

void Strategy::Strategy_Armors(vector<Armor> armors, Mat frame)
{

    if (armors.empty())
    {
        sendData.yaw = 0;
        sendData.pitch = 0;
    }
    else if (!armors.empty() && !is_tracking())
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

        if (this->tracking_armor.dis_count > 2 || tracking_armor.PnP_data.distance > 500)
        {
            tracking_armor.is_tracked = false;
        }
    }
    cout << 1 << endl;
    float total_yaw = 0.f;
    float total_pitch = 0.f;

    if (this->is_tracking())
    {
        for (int i = 0; i < armors.size(); i++)
        {
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
}
