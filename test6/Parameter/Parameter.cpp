/**
 * @file Parameter.cpp
 * @author 徐英杰 (541223130@qq.com)
 * @brief 
 * @version 1.0
 * @date 2021-10-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include"Parameter.h"
#include <iostream>

using namespace std;

const string CMAKE_CONFIG_FILES = "/usr/local/etc/";

/**
 * @brief 从yml文件中读取灯条参数
 * 
 * @param param 文件路径
 */

LightParam::LightParam(const string &param)
{
    FileStorage lbp(param, FileStorage::READ);
    lbp["MAX_RATIO"] >> this->MAX_RATIO;
    lbp["MIN_RATIO"] >> this->MIN_RATIO;
    lbp["MIN_AREA_RATIO"] >> this->MIN_AREA_RATIO;
    lbp["MAX_ANGLE"] >> this->MAX_ANGLE;
    lbp["MIN_AREA"] >> this->MIN_AREA;
}
LightParam light_param("/home/lynn/桌面/-/test6/setting_file/LightParam.yml");

/**
 * @brief 从yml文件中读取装甲板参数
 * 
 * @param param 文件路径
 */
ArmorParam::ArmorParam(const string &param)
{
    FileStorage arm(param, FileStorage::READ);
    arm["MAX_CROSS_RATIO"] >> this->MAX_CROSS_RATIO;
    arm["MAX_DETLA_LENGTH"] >> this->MAX_DETLA_LENGTH;
    arm["MAX_RATIO"] >> this->MAX_RATIO;
    arm["MIN_RATIO"] >> this->MIN_RATIO;
    arm["MAX_ANGLE"] >> this->MAX_ANGLE;
    arm["MIN_X_LENGTH_RATIO"] >> this->MIN_X_LENGTH_RATIO;
    arm["MAX_Y_LENGTH_RATIO"] >> this->MAX_Y_LENGTH_RATIO;
}
ArmorParam armor_param("/home/lynn/桌面/-/test6/setting_file/ArmorParam.yml");

/**
 * @brief 从yml文件中读取相机参数
 * 
 * @param param 文件路径
 */
CameraParam::CameraParam(const string &param)
{
    FileStorage cam(param, FileStorage::READ);

    cam["cam_exposure"] >> this->cam_exposure;
    cam["cam_gamma"] >> this->cam_gamma;
    cam["cam_contrast"] >> this->cam_contrast;
    cam["cam_Bgain"] >> this->cam_Bgain;
    cam["cam_Ggain"] >> this->cam_Ggain;
    cam["cam_Rgain"] >> this->cam_Rgain;

    cam["image_width"] >> this->image_width;
    cam["image_height"] >> this->image_height;
}
CameraParam camera_param("/home/lynn/桌面/-/test6/setting_file/CameraParam.yml");

/**
 * @brief 从yml文件中读取其他参数
 * 
 * @param param 文件路径
 */
OtherParam::OtherParam(const string &param)
{
    FileStorage oth(param, FileStorage::READ);
    oth["GRAY_THRESHOLD_RED"] >> this->GRAY_THRESHOLD_RED;
    oth["GRAY_THRESHOLD_BLUE"] >> this->GRAY_THRESHOLD_BLUE;

    oth["INIT_PITCH"] >> this->INIT_PITCH;
    oth["INIT_YAW"] >> this->INIT_YAW;
    oth["INIT_COLOR_MODE"] >> this->INIT_COLOR_MODE;
    oth["INIT_SHOOT_SPEED"] >> this->INIT_SHOOT_SPEED;
    oth["INIT_MODE"] >> this->INIT_MODE;
    oth["point_scale"] >> this->point_scale;
}
OtherParam other_param("/home/lynn/桌面/-/test6/setting_file/OtherParam.yml");


/**
 * @brief 从yml文件中读取决策参数
 * 
 * @param param 文件路径
 */
StrategyParam::StrategyParam(const string &param)
{
    FileStorage strategy(param, FileStorage::READ);
    strategy["SENTRY_TRACK_FRAMES"] >> this->SENTRY_TRACK_FRAMES;
    strategy["NORMAL_TRACK_FRAMES"] >> this->NORMAL_TRACK_FRAMES;
    strategy["RECORD_FRAMES"] >> this->RECORD_FRAMES;
    strategy["MAX_DIS"] >> this->MAX_DIS;
    strategy["MAX_DIS_DIFF"] >> this->MAX_DIS_DIFF;
    strategy["GYRO_RADIUS_RATIO"] >> this->GYRO_RADIUS_RATIO;
    strategy["NORMAL_RADIUS_RATIO"] >> this->NORMAL_RADIUS_RATIO;
    strategy["SHOOT_CONFIDENCE_LEVEL"] >> this->SHOOT_CONFIDENCE_LEVEL;
    strategy["SHOOT_SPEED_15_BIAS"] >> this->SHOOT_SPEED_15_BIAS;
    strategy["SHOOT_SPEED_18_BIAS"] >> this->SHOOT_SPEED_18_BIAS;
    strategy["SHOOT_SPEED_30_BIAS"] >> this->SHOOT_SPEED_30_BIAS;
}
StrategyParam strategy_param("/home/lynn/桌面/-/test6/setting_file/OtherParam.yml");