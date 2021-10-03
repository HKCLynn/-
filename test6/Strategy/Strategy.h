

#ifndef STATEGY_H 
#define STATEGY_H 

#include <opencv2/opencv.hpp>
#include "Armor.h"


using namespace std;
using namespace cv;


struct DataStruct
{
    float yaw;   // yaw 角度   -- 正值表示枪口往右偏移
    float pitch; // pitch 角度 -- 正值表示枪口往下偏移
    uint8_t End = 0;
};


class Strategy
{
public:
    Armor tracking_armor;
    DataStruct sendData;
    void Strategy_Armors(vector<Armor> &tracking_armor,Mat &frame);
    bool changeTracker();

};

#endif
