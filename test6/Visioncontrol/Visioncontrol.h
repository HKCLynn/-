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
#include "Detector.h"
#include "Parameter.h"
#include "RMVideoCapture.h"

// 识别模式
enum Detect_mode
{
    ARMOR_DETECT = 0U,   // 装甲模式
    RUNE_DETECT = 1U,    // 神符模式
    DISTURB_DETECT = 3U, // 干扰模式
    PILOT_DETECT = 4U    // 引导模式
};

// 相机模式
enum Camera_mode
{
    NORMAL_CAM = 0, // 普通相机
    RUNE_CAM = 1,   // 普通相机打符曝光
    SLING_CAM = 2   // 吊射相机
};

class Visioncontrol
{
public:
    unique_ptr<Detector> detector;
    string path;
    unique_ptr<VideoCapture> capture = nullptr;
    Mat frame;              // 每帧图像
    SerialPort port;        // 串口对象
    GyroScope port_manager; // 串口管理对象
    GyroData gyro_data;     // 接收数据
    DataStruct send_data;   //发送数据
    uint8_t __mode;         // 接收的模式
    uint8_t __detectId;     // 识别模式
    uint8_t __cameraId=0U;     // 相机模式
    bool run(ArmorTracker &tracker,Strategy &strategy); // 处理函数 // 设置模式
    inline void setId(uint8_t mode) { __mode = mode; }

    void initCamera(); // 初始化相机
    void setDetector();       // 设置检测器
    void setCamera();
};
