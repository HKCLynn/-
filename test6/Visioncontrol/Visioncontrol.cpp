#include "SerialPort.h"
#include"Visioncontrol.h"
#include<opencv2/opencv.hpp>

using namespace std;

VisionControl::~VisionControl()
{
    // 释放资源
    this->capture.release();
    this->capture = nullptr;

}