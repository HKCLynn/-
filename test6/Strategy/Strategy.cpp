#pragma once

#include <opencv2/opencv.hpp>
#include "Armor.h"
#include "GyroScope.h"
#include "Strategy.h"

using namespace std;
using namespace cv;

void Strategy::Strategy_Armors(vector<Point2f> centers)
{
    if (centers.empty())
    {
        pSendData->yaw = 0;
        pSendData->pitch = 0;
    }
    else if(!centers.empty()&&!has_armor)
    {

    }
}

