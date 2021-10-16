/**
 * @file Parameter.h
 * @author 徐英杰 (541223130@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-10-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

// ---------【编译安装路径】---------
extern const string CMAKE_CONFIG_FILES;

class LightParam
{
public:
    float MAX_RATIO = 10;       //灯条最大长宽比
    float MIN_RATIO = 1.5;      //灯条最小长宽比
    float MIN_AREA_RATIO = 0.6; //灯条最小占比
    float MAX_ANGLE = 45;       //灯条倾斜最大角度
    int MIN_AREA = 10;          //灯条最小面积

    LightParam(const string &param);
};

class ArmorParam
{
public:
    float MAX_CROSS_RATIO = 0.115;  //对角线和装甲板高度的比值
    float MAX_DETLA_LENGTH = 20;    //两灯条高度的最大差值
    float MAX_RATIO = 3.3;          //装甲板最大长宽比
    float MIN_RATIO = 1.5;          //装甲板最小长宽比
    float MAX_ANGLE = 5.5;          //两灯条倾斜角度的差值
    float MIN_X_LENGTH_RATIO = 0.5; //两灯条中心点x的差值和高度的比值
    float MAX_Y_LENGTH_RATIO = 1;   //亮灯条中心点y的差值和高度的比值

    ArmorParam(const string &param);
};

// 相机参数
struct CameraParam
{
    int image_width = 1280;  // 相机宽度和高度
    int image_height = 1024; // 相机宽度和高度
    int cam_exposure = 300;  // 相机曝光
    int cam_gamma = 100;     // 相机 Gamma 值
    int cam_contrast = 100;  // 相机对比度
    int cam_Bgain = 100;     // 相机蓝色增益
    int cam_Ggain = 121;     // 相机绿色增益
    int cam_Rgain = 150;     // 相机红色增益

    CameraParam(const string &param);
};

struct OtherParam
{
    int GRAY_THRESHOLD_RED = 100; // pre_handle -- 蓝色阈值
    int GRAY_THRESHOLD_BLUE = 60; // pre_handle -- 红色阈值
    float INIT_PITCH = 0;         // GyroScope_init - 陀螺仪 - pitch 角度
    float INIT_YAW = 0;           // GyroScope_init - 陀螺仪 - yaw 角度
    uint8_t INIT_COLOR_MODE = 0;  // GyroScope_init - 己方颜色
    uint8_t INIT_SHOOT_SPEED = 0; // GyroScope_init - 枪口射速
    uint8_t INIT_MODE = 30;       // GyroScope_init - 模式
    int point_scale = 1;          // 调试显示的点大小

    OtherParam(const string &param);
};

struct StrategyParam
{
    int SENTRY_TRACK_FRAMES = 10;       // Armor - 哨兵模式追踪帧数
    int NORMAL_TRACK_FRAMES = 5;        // Armor - 普通模式
    int RECORD_FRAMES = 12;             // Armor - 时间序列记录最大帧数
    int MAX_DIS = 8000;                 // Armor - 最远距离
    int MAX_DIS_DIFF = 500;             // Armor - 最大切换装甲板距离差值
    float GYRO_RADIUS_RATIO = 10;       // 击打小陀螺时射击半径比值
    float NORMAL_RADIUS_RATIO = 1.1;    // 普通模式时射击半径比值
    float SHOOT_CONFIDENCE_LEVEL = 0.8; // 普通模式下预测点的置信度
    int SHOOT_SPEED_15_BIAS = 1;        // 15m/s 弹速负偏置
    int SHOOT_SPEED_18_BIAS = 1;        // 18m/s 弹速负偏置
    int SHOOT_SPEED_30_BIAS = 2;        // 30m/s 弹速负偏置

    StrategyParam(const string &param);
};

extern LightParam light_param;
extern ArmorParam armor_param;
extern CameraParam camera_param;
extern OtherParam other_param;
extern StrategyParam strategy_param;
