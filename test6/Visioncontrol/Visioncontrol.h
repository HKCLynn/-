/**
 * @file Visioncontrol.h
 * @author 徐英杰 (541223130@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-10-03
 * 
 */


#include <opencv2/opencv.hpp>
#include "SerialPort.h"
#include "GyroScope.h"
#include "Deal.h"



class Visioncontrol
{
public:
    unique_ptr<VideoCapture> capture = nullptr;
    Mat frame;                                  // 每帧图像
    SerialPort port;                            // 串口对象
    GyroScope port_manager;                     // 串口管理对象
    GyroData gyro_data;                         // 接收数据
    ~VisionControl();                           // 析构函数
    bool run();                                 // 处理函数 // 设置模式

    void initCamera(int _id); // 初始化相机
    void setDetector();       // 设置检测器
    void setCamera();         // 设置相机
    void initCamera();
};
