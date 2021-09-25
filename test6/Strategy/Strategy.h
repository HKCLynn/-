

#pragma once

#include <opencv2/opencv.hpp>
#include "GyroScope.h"
using namespace std;
using namespace cv;

class Strategy
{
public:
    Point2f tracking_armor=Point2f(-1,-1);
    DataStruct *pSendData;
    void Strategy_Armors(vector<Point2f> centers);
    bool has_armor(){if(tracking_armor==Point2f(-1,-1))
                        return false;
                        else return true;}
    bool changeTracker();
    void judgeShootGryo(float &, float &);
};

struct DataStruct
{
    float yaw;   // yaw 角度   -- 正值表示枪口往右偏移
    float pitch; // pitch 角度 -- 正值表示枪口往下偏移
    uint8_t End = 0;
};
