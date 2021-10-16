/**
 * @file test6.cpp
 * @author 徐英杰 (541223130@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-10-05
 * 
 * 
 */

#include <iostream>
#include <stdio.h>
#include "Visioncontrol.h"



using namespace std;
using namespace cv;

int main(int argc, char *argv[])
{
    //追踪装甲板
    ArmorTracker tracker;
    //决策信息
    Strategy strategy;
    Visioncontrol visioncontrol;
    while (1)
    {
        if (!visioncontrol.run(tracker,strategy))
            return 0;
    }
}